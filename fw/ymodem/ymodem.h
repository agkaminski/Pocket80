/* Pocket80 ymodem receiver
 * A.K. 2025
 */

#ifndef _YMODEM_H_
#define _YMODEM_H_

#include <stdint.h>

#define YMODEM_BLK_SZ 128

#ifndef SOH
#define SOH 0x01
#endif
#ifndef STX
#define STX 0x02
#endif
#ifndef EOT
#define EOT 0x04
#endif
#ifndef ACK
#define ACK 0x06
#endif
#ifndef NAK
#define NAK 0x15
#endif

struct ymodem_packet {
	uint16_t header;
	uint8_t blk;
	uint8_t nblk;
	uint8_t data[YMODEM_BLK_SZ];
	uint16_t crc;
};

int8_t ymodem_verify(const struct ymodem_packet *packet);

#endif
