#include "analog_signals.h"

#define PI 	3.14159265358979323846
#define TAU (2.0 * PI)

int16_t signal[46876];
int number_samples = sizeof(signal) / sizeof(signal[0]);

void analog_signal_create(uint32_t frequency) {
	
	int i = 0;
	
	while(i < number_samples) {
			double t = ((double)i / 2.0) / ((double)number_samples);
			signal[i] = 32767 * sin((double)frequency * TAU * t); // left
			signal[i+1] = signal[i]; // right
			i += 2;
	}
}

void analog_signal_generate(void) {
	HAL_I2S_Transmit(&hi2s3, (uint16_t*)signal, number_samples, HAL_MAX_DELAY);
}
