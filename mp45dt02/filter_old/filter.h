#ifndef __FILTER_H
#define __FILTER_H

#include "main.h"
#include "pdm_filter.h"

#define HTONS(A)  ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))

void pdm_filter_init(void);
void get_filtered_values(uint8_t *input_data, int16_t *output_data);


#endif /* __FILTER_H */


