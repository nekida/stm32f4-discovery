1. TIM - TIM4 - Clock Source: Internal Clock. Channel3: PWM Generation CH3 (на этой GPIO LED5). TIM4 "сидит" на шине APB1, ее тактовая частота 84 МГц -> Prescaler: 8400 - 1, Counter Period: 10000 - 1. Получаем период счета таймера T = 1 с.
2. ШИМ импульс 5000, что дает частоту мигания 1 Гц: PWM Generation Channel3 -> Pulse (16 bit value): 5000. По сути, это значение - время нахождения в логической 1: Counter Period = 1000 мс, Pulse = 5000 -> ножка в единице 500 мс (Pulse = 500 -> 50 мс, Pulse = 7500 -> 750 мс).
3. Неблокирующий процесс: HAL_TIM_PWM_Start.
