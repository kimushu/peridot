#ifndef AUDIO_H_
#define AUDIO_H_

typedef void (*payload_decoder_t)(const void *, int);

payload_decoder_t audio_init(const void *header);

#endif /* AUDIO_H_ */
