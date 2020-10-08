1. RTC -> Alarm A -> Internal Alarm.
2. LED3, LED4 on.
3. RTC -> Configuration -> Alarm A -> Seconds = 1.
4. NVIC -> RTC alarms A and B interrupt through EXTI 17.
5. RTC можно сохранить во время Reset (режимы ok LP). CubeMX не включает RTC по умолчанию, нужно добавить HAL_PWR_EnableBkUpAccess() и __HAL_RCC_RTC_ENABLE() перед вызовом MX_RTC_Init().
6. Установите первый будильник на 1 с в MX_RTC_Init.
7. Создаем обработчик прерывания RTC и перенастраиваем время сигнала тревоги A:
	-HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
	-HAL_RTC_GetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Alarm, uint32_t Format)
	-HAL_RTC_SetAlarm_IT(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format)
8. Индикация аварийных сигналов RTC будет осуществляться светодиодом: HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin).
9. Включить RTC:
	HAL_PWR_EnableBkUpAccess();	//включить доступ к домену резервного копирования PWR (RTC,BKReg)
	__HAL_RCC_RTC_ENABLE();		//включить RTC. Не создается кубом, потому что RTC может работать
	MX_GPIO_Init();
	MX_RTC_Init();
10. При отсутствии маскирования дней/часов/минут - диод мигает 1 минуту (1 с горит, 1 с погашен).
