1. TIM - TIM1 - Clock Source: Internal Clock. TIM1 "сидит" на шине APB2, ее тактовая частота 168 МГц -> Prescaler: 16800 - 1, Counter Period: 10000 - 1. Получаем период счета таймера T = 1 с.
2. NVIC -> TIM1 update interrupt and TIM10 global interrupt.
3. Алгоритм: обработчик TIM1_UP_TIM10_IRQHandler вызывает HAL_TIM_IRQHandler, который в зависимости от своего результата вызовет HAL_TIM_PeriodElapsedCallback (по истечению периода) или HAL_TIM_ErrorCallback в случае ошибки.
4. HAL_TIM_Base_Start_IT() запускает таймер в режиме прерывания.
5. В Callback'е voidHAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) переключаем состояние LED4.