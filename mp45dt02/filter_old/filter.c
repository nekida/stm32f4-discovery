#include "filter.h"

PDMFilter_InitStruct Filter;
uint16_t MicGain = 30;

void pdm_filter_init(void) {
	__HAL_RCC_CRC_CLK_ENABLE();
	Filter.LP_HZ = 8000.0;
	Filter.HP_HZ = 200.0;

	Filter.Fs = 16000;
	Filter.Out_MicChannels = 1;
	Filter.In_MicChannels = 1;
    
  PDM_Filter_Init((PDMFilter_InitStruct *)&Filter);
}

void get_filtered_values(uint8_t *input_data, int16_t *output_data) {
	PDM_Filter_64_LSB(input_data, (uint16_t *)output_data, 80 , (PDMFilter_InitStruct *)&Filter);
}
