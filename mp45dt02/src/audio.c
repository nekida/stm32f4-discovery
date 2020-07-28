#include "audio.h"

int16_t 	pcm_buffer[SIZE_PCM_BUF]; 
uint16_t 	mic_dma_pdm_buffer[SIZE_DMA_INPUT];
int16_t 	audiodata[SIZE_PSEUDO_STEREO_BUF];

void audio_filter_out(void) {
	static uint16_t pdm_temp_buffer[SIZE_DMA_INPUT];
  uint8_t i;

	for (i = 0; i < SIZE_DMA_INPUT; i++)
		pdm_temp_buffer[i] = HTONS(mic_dma_pdm_buffer[i]);
  
	get_filtered_values((uint8_t *)pdm_temp_buffer, pcm_buffer);
}

void audio_play(void) {
	HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*)audiodata, SIZE_PSEUDO_STEREO_BUF);
}

void audio_make_pseudo_stereo(void) {
  uint8_t i;
	
  for (i = 0; i < SIZE_PSEUDO_STEREO_BUF; i++) {
		audiodata[i] = pcm_buffer[i >> 1]; 
	}
}
