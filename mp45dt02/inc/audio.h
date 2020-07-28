#ifndef __AUDIO_H
#define __AUDIO_H

#include "main.h"
#include "filter.h"
#include "i2s.h"

#define SIZE_DMA_INPUT					64
#define SIZE_PCM_BUF						16
#define SIZE_PSEUDO_STEREO_BUF	32

void audio_filter_out(void);
void audio_make_pseudo_stereo(void);
void audio_play(void);


#endif /* __AUDIO_H */


