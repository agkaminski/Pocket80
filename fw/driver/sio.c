/* Pocket80 Z80 SIO driver
 * A.K. 2025
 */

#include <stdint.h>

#include "sio.h"
#include "interrupt.h"

extern void dlprint(const char *str);

extern void sio_reg_write(uint8_t channel, uint8_t reg, uint8_t val);
extern uint8_t sio_reg_read(uint8_t channel, uint8_t reg);
extern void sio_data_write(uint8_t channel, uint8_t data);
extern uint8_t sio_data_read(uint8_t channel);

#define FIFO_SIZE 32

struct fifo {
	uint8_t data[FIFO_SIZE];
	uint8_t rd, wr;
};

static int8_t _fifo_push(struct fifo *fifo, uint8_t c, uint8_t overwrite)
{
	uint8_t nwr = (fifo->wr + 1) % sizeof(fifo->data);

	if (nwr == fifo->rd) {
		if (overwrite) {
			fifo->rd = (fifo->rd + 1) % sizeof(fifo->data);
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
	fifo->rd = (fifo->rd + 1) % sizeof(fifo->data);

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
//	struct uart *uart = &g_uart[channel];

	while (cnt < bufflen) {
		int8_t err = 0;

//		char msg[] = "PRINT 0";
//		msg[6] += cnt;
//		dlprint(msg);

//		DI();
		if (!(g_uart[channel].tx_busy)) {
			g_uart[channel].tx_busy = 1;
			sio_data_write(channel, data[cnt++]);
		}
//		else {
//			err = _fifo_push(&uart->tx, data[cnt], 0);
//		}
//		EI();

//		if (!err) {
//			++cnt;
//		}

		dlprint("DUPA");
	}
}

void sio_read(uint8_t channel, void *buff, size_t bufflen)
{
	uint8_t *data = buff;

	while (bufflen) {
		if (fifo_pop(&g_uart[channel].rx, data) == 0) {
			++data;
			--bufflen;
		}
	}
}

size_t sio_read_nb(uint8_t channel, void *buff, size_t bufflen)
{
	uint8_t *data = buff;
	size_t cnt;

	for (cnt = 0; cnt < bufflen; ++cnt) {
		if (fifo_pop(&g_uart[channel].rx, &data[cnt]) < 0) {
			break;
		}
	}

	return cnt;
}

void sio_irq_tx_empty(uint8_t channel)
{
	static uint8_t cnt = 0;

//	uint8_t data;

//	dlprint("IRQ TX");

//	if (_fifo_pop(&g_uart[channel].tx, &data) == 0) {
//		sio_data_write(channel, data);
//	}
//	else {
		g_uart[channel].tx_busy = 0;
//	}

	/* Reset TX interrupt, as there's nothing more to send */
	sio_reg_write(channel, 0, 5 << 3);

	if (!channel) {
		/* Obligatory channel A interrupt reset */
		sio_reg_write(channel, 0, 7 << 3);
	}
}

void sio_irq_rx_avail(uint8_t channel)
{
//	dlprint("IRQ RX");

	uint8_t data = sio_data_read(channel);
	_fifo_push(&g_uart[channel].rx, data, 1);

	if (!channel) {
		/* Obligatory channel A interrupt reset */
		sio_reg_write(channel, 0, 7 << 3);
	}
}

void sio_irq_special(uint8_t channel)
{
//	dlprint("IRQ SP");

	/* TODO? */

	if (!channel) {
		/* Obligatory channel A interrupt reset */
		sio_reg_write(channel, 0, 7 << 3);
	}
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
