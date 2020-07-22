#include "notes.h"

void MX_TIM1_Init_custom(uint32_t val_of_period) {
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1680;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = val_of_period;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
    Error_Handler();
  }
}

void note_do_func(void) {
	HAL_TIM_Base_Stop_IT(&htim1);
	MX_TIM1_Init_custom(190);
	HAL_TIM_Base_Start_IT(&htim1);
}

void note_re_func(void) {
	HAL_TIM_Base_Stop_IT(&htim1);
	MX_TIM1_Init_custom(169);
	HAL_TIM_Base_Start_IT(&htim1);
}

void note_mi_func(void) {
	HAL_TIM_Base_Stop_IT(&htim1);
	MX_TIM1_Init_custom(151);
	HAL_TIM_Base_Start_IT(&htim1);
}

void note_fa_func(void) {
	HAL_TIM_Base_Stop_IT(&htim1);
	MX_TIM1_Init_custom(142);
	HAL_TIM_Base_Start_IT(&htim1);
}

void note_sol_func(void) {
	HAL_TIM_Base_Stop_IT(&htim1);
	MX_TIM1_Init_custom(126);
	HAL_TIM_Base_Start_IT(&htim1);
}

void note_la_func(void) {
	HAL_TIM_Base_Stop_IT(&htim1);
	MX_TIM1_Init_custom(113);
	HAL_TIM_Base_Start_IT(&htim1);
}

void note_si_func(void) {
	HAL_TIM_Base_Stop_IT(&htim1);
	MX_TIM1_Init_custom(100);
	HAL_TIM_Base_Start_IT(&htim1);
}
