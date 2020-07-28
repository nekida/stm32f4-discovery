#include "filter.h"

PDM_Filter_Handler_t  PDM_FilterHandler;
PDM_Filter_Config_t   PDM_FilterConfig;

void pdm_filter_init(void) {
	__HAL_RCC_CRC_CLK_ENABLE();
	PDM_FilterHandler.bit_order  = PDM_FILTER_BIT_ORDER_LSB;
	PDM_FilterHandler.endianness = PDM_FILTER_ENDIANNESS_LE;
	PDM_FilterHandler.high_pass_tap = 2122358088;
	PDM_FilterHandler.out_ptr_channels = 1;
	PDM_FilterHandler.in_ptr_channels  = 1;
	PDM_Filter_Init((PDM_Filter_Handler_t *)(&PDM_FilterHandler));
	
	PDM_FilterConfig.output_samples_number = 16;
	PDM_FilterConfig.mic_gain = 24;
	PDM_FilterConfig.decimation_factor = PDM_FILTER_DEC_FACTOR_64;
	PDM_Filter_setConfig((PDM_Filter_Handler_t *)&PDM_FilterHandler, &PDM_FilterConfig);
}

void get_filtered_values(uint8_t *input_data, int16_t *output_data) {
	PDM_Filter(input_data, output_data, &PDM_FilterHandler);
}
