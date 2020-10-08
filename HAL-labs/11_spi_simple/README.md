1. Используется SPI2. MOSI и MISO закорочены.
2. Режим: Full-Duplex Master. GPIO SPI2 в Very High (CubeMX делает автоматически).
3. Используется блокирующая функция HAL_SPI_TransmitReceive() (HAL_OK/HAL_ERROR/HAL_ERROR).
4. На высоких скоростях передачи (HCLK = 168 МГц, PCLK = 42 МГц, Prescaler SPI = 2, Baud Rate SPI = 21 MBits/s) происходит потеря данных и вылет в Callback HAL_SPI_ErrorCallback.