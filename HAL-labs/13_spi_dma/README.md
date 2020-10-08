1. Используется SPI2. MOSI и MISO закорочены.
2. Режим: Full-Duplex Master. GPIO SPI2 в Very High (CubeMX делает автоматически).
3. Настройки DMA
Mode: Normal
Data Width: Byte
Increment Address: Memory
4. Алгоритм: периферийные инициализации, включая инициализацию потока DMA -> HAL_SPI_TransmitReceive_DMA (HAL_OK/HAL_ERROR/HAL_ERROR) - старт процесса с DMA, при завешении передачи генерируется прерывание -> обработчик прерывания DMA2_Stream0_IRQ Handler запускает -> HAL_DMA_IRQHandler запускает -> Callback HAL_SPI_TxRxCpltCallback()/HAL_SPI_ErrorCallback().
5. В этом случае на высокой скорости (HCLK = 168 МГц, PCLK = 42 МГц, Prescaler SPI = 2, Baud Rate SPI = 21 MBits/s) данные передались без проблем.
6. По непонятным причинам CubeMX отказывался вызывать в DMA1_Stream3_IRQHandler()/DMA1_Stream4_IRQHandler() обработчики HAL_DMA_IRQHandler(&hdma_spi2_rx)/HAL_DMA_IRQHandler(&hdma_spi2_tx). Пришлось пересоздать проект.