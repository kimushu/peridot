#include <stdint.h>
#include <stddef.h>

#include <system.h>
#include <io.h>
#include "audio.h"

#define SOUND_REG_STATUS	(0*4)
#define SOUND_REG_VOLUME	(1*4)
#define SOUND_REG_FS8DIV	(2*4)
#define SOUND_REG_FIFOWR	(3*4)

#define AUDIO_PCM8			(0x0001)

typedef struct {
	unsigned char a_codec;
	unsigned char a_channel;
	unsigned short a_rate;
} audio_ck_header;

static void audio_decoder(const void *buffer, int length)
{
	int samples;
	const uint8_t *data;

	samples = *((const uint16_t *)buffer);
	data = ((const uint8_t *)buffer) + 2;

	if (samples > (length - 2)) {
		samples = length - 2;
	}

	for (; samples > 0; --samples)
	{
		IOWR_8DIRECT(SOUND_BASE, SOUND_REG_FIFOWR, *data++);
	}
}

payload_decoder_t audio_init(const void *header)
{
	int divider;
	const audio_ck_header *h = (const audio_ck_header *)header;

	if (h->a_codec != AUDIO_PCM8) {
		return NULL;
	}

	if (h->a_channel != 1) {
		return NULL;
	}

	divider = (SOUND_FREQ / (h->a_rate * 8)) - 129;
	if (divider < 0 || divider > 255) {
		return NULL;
	}

	// FIFO reset
	IOWR_8DIRECT(SOUND_BASE, SOUND_REG_STATUS, 0x01);
	IOWR_8DIRECT(SOUND_BASE, SOUND_REG_STATUS, 0x00);

	// Volume set
	IOWR_8DIRECT(SOUND_BASE, SOUND_REG_VOLUME, 64);

	// Divider set
	IOWR_8DIRECT(SOUND_BASE, SOUND_REG_FS8DIV, divider);

	return audio_decoder;
}

