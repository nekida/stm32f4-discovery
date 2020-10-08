1. Используется USART2, USART3
USART2		USART3
TX(PA2)-----RX(PD9)	
RX(PA3)-----TX(PD8)
2. Скорость GPIO USART в Very High
3. Инициализация: CubeMX создает функцию, которая обрабатывает ициализацию UART (MX_USART2_UART_Init()/MX_USART3_UART_Init()) -> CubeMX заполняет структуру UART параметрами, которые были выбиранв в окне конфигурации -> функция записывает параметры из структуры в регистры USART2/USART3 -> необязательный Callback от HAL_UART_Initfunction, по умолчанию пустая weak функция - HAL_UART_MspInitcallback() -> CubeMX настраивает GPIO и включает систему синхронизации (RCC) уартов и NVIC.
4. Функции HAL_UART_Transmit()/HAL_UART_Receive() являются блокирующими на время Timeout.