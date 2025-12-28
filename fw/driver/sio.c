/* Pocket80 Z80 SIO driver
 * A.K. 2025
 */

#include <stdint.h>

#include "sio.h"
#include "interrupt.h"

extern void _sio_reg_write(uint8_t channel, uint16_t regval);
extern uint8_t sio_reg_read(uint8_t channel, uint8_t reg);
extern void sio_data_write(uint8_t channel, uint8_t data);
extern uint8_t sio_data_read(uint8_t channel);

#define sio_reg_write(channel, reg, val) _sio_reg_write((channel), (uint16_t)(reg) | ((uint16_t)(val) << 8))

#define FIFO_SIZE 32

struct fifo {
	uint8_t data[FIFO_SIZE];
	uint8_t rd, wr;
};

static uint8_t fifo_incmod(uint8_t ptr)
{
	return (ptr + 1) & ~(FIFO_SIZE - 1);
}

static int8_t _fifo_push(struct fifo *fifo, uint8_t c, uint8_t overwrite)
{
	uint8_t nwr = fifo_incmod(fifo->wr);

	if (nwr == fifo->rd) {
		if (overwrite) {
			fifo->rd = fifo_incmod(fifo->rd);
		}
		else {
			return -1;
		}
	}

	fifo->data[fifo->wr] = c;
	fifo->wr = nwr;

	return 0;
}

static int8_t _fifo_pop(struct fifo *fifo, uint8_t *c)
{
	if (fifo->wr == fifo->rd) {
		return -1;
	}

	*c = fifo->data[fifo->rd];
	fifo->rd = fifo_incmod(fifo->rd);

	return 0;
}

static int8_t fifo_pop(struct fifo *fifo, uint8_t *c)
{
	DI();
	int8_t ret = _fifo_pop(fifo, c);
	EI();

	return ret;
}

struct uart {
	struct fifo tx;
	struct fifo rx;
	volatile uint8_t tx_busy;
};

struct uart g_uart[2];

void sio_write(uint8_t channel, const void *buff, size_t bufflen)
{
	const uint8_t *data = buff;
	size_t cnt = 0;
	struct uart *uart = &g_uart[channel];

	while (cnt < bufflen) {
		int8_t err = 0;

		DI();
		if (!(uart->tx_busy)) {
			uart->tx_busy = 1;
			sio_data_write(channel, data[cnt]);
		}
		else {
			err = _fifo_push(&uart->tx, data[cnt], 0);
		}
		EI();

		if (!err) {
			++cnt;
		}
	}
}

static size_t _sio_read(uint8_t channel, void *buff, size_t bufflen, int8_t nonblock)
{
	uint8_t *data = buff;
	size_t cnt = 0;

	while (cnt < bufflen) {
		if (fifo_pop(&g_uart[channel].rx, data + cnt) == 0) {
			++cnt;
		}
		else if (nonblock) {
			break;
		}
	}

	return cnt;
}

void sio_read(uint8_t channel, void *buff, size_t bufflen)
{
	(void)_sio_read(channel, buff, bufflen, 0);
}

size_t sio_read_nb(uint8_t channel, void *buff, size_t bufflen)
{
	return _sio_read(channel, buff, bufflen, 0);
}

void sio_irq_tx_empty(uint8_t channel)
{
	uint8_t data;

	if (_fifo_pop(&g_uart[channel].tx, &data) == 0) {
		sio_data_write(channel, data);
	}
	else {
		/* Reset TX interrupt as there's nothing more to send.
		 * Otherwise we'll die looping here */
		sio_reg_write(channel, 0, 5 << 3);
		g_uart[channel].tx_busy = 0;
	}
}

void sio_irq_rx_avail(uint8_t channel)
{
	uint8_t data = sio_data_read(channel);
	_fifo_push(&g_uart[channel].rx, data, 1);
}

void sio_irq_special(uint8_t channel)
{
	/* TODO? */
	/* Error reset */
	sio_reg_write(channel, 0, 6 << 3);
}

void sio_init(uint8_t vector)
{
	/* Channel B */
	/* Reset */
	sio_reg_write(1, 0, 0x18);
	sio_reg_write(1, 0, 0x10);

	/* Interrupt vector (channel B only!) */
	sio_reg_write(1, 2, vector);

	/* 1 stop bit, x16 clock, no parity */
	sio_reg_write(1, 4, 0x44);

	/* Receiver enable, 8 bits, no BS */
	sio_reg_write(1, 3, 0xc1);

	/* Transmitter enable, 8 bits, no BS */
	sio_reg_write(1, 5, 0x68);

	/* Interrupts */
	sio_reg_write(1, 1, 0x16); /* + Status Affects Vector */

	/* Channel A */
	/* Reset */
	sio_reg_write(0, 0, 0x18);
	sio_reg_write(0, 0, 0x10);

	/* 1 stop bit, x16 clock, no parity */
	sio_reg_write(0, 4, 0x44);

	/* Receiver enable, 8 bits, no BS */
	sio_reg_write(0, 3, 0xc1);

	/* Transmitter enable, 8 bits, no BS */
	sio_reg_write(0, 5, 0x68);

	/* Interrupts */
	sio_reg_write(0, 1, 0x12);
}
