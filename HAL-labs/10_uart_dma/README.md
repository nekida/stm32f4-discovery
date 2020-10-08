1. Используем USART3, включаем DMA (Mode: Normal, Data Width: Byte), включаем прерывания DMA для передачи и приема USART. Замыкаем RX-TX.
2. CubeMX генерирует код инициализации периферии, влючая DMA поток, обработчик прерывания DMA_Stream_IRQ (создается в файле stm32f4xx_it.c) и обработчик HAL_DMA_IRQHandler (создается в файле stm32f4xx_hal_dma.c и затем линкуется с файлом stm32f4xx_hal_uart.c). 
3. Запуск процесса с DMA с генерацией прерывания по окончании передачи - HAL_UART_Receive_DMA().
4. Callback функции, вызываемые из HAL_DMA_IRQHandler - HAL_UART_RxCpltCallback()/HAL_UART_ErrorCallback().
5. Буфер приема через UART с DMA, программа не блокируется.
6. Прерывание DMA_Stream_IRQ указывает на получение половины/всего буфера DMA или на обнаружение ошибки DMA.
7. HAL_DMA_IRQHandler - информация о прерывании процесса от DMA и UART.
8. Буфер данных полон - HAL_UART_RxCpltCallback(), обнаружена ошиба - HAL_UART_ErrorCallback().
9. Алгоритм: инициализация USART3 и DMA -> принимаем буфер по DMA (HAL_UART_Receive_DMA()) -> буфер заполнен (DMA_Stream_IRQ Handler) -> прерывание процесса (HAL_DMA_IRQHandler) -> вызов Callback функций (буфер принят - HAL_UART_RxCpltCallback() / ошибка приема - HAL_UART_ErrorCallback).