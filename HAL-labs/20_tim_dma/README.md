1. TIM - TIM1 - Clock Source: Internal Clock. TIM1 "сидит" на шине APB2, ее тактовая частота 168 МГц -> Prescaler: 16800 - 1, Counter Period: 10000 - 1. Получаем период счета таймера T = 1 с.
2. DMA Settings -> ADD -> TIM1_UP_DMA -> Mode: Circular, Data Width: Half Word, Memory Increment, Direction: M2P.
3. extern DMA_HandleTypeDef hdma_tim1_up; в файле tim.h для использования в main.h.
4. Перед запуском HAL_DMA_Start() - __HAL_TIM_ENABLE_DMA(&htim1, TIM_DMA_UPDATE); - это триггер от TIM1 для DMA передачи.
5. Программа: настроен циклический режим передачи на пол слова в DMA. Первой транзакцией записывается в порт GPIOD (регистр ODR) значение GPIO_PIN_12, потом 0х0000, что приводит к переключению 12 ножки порта D раз в 1 секунду (по событию переполнения TIM1).