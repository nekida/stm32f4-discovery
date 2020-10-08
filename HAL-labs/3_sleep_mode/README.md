1. Программа переводит процессорное ядро в режим SLEEP, пока не будет нажата кнопка пробуждения. Далее запускается "мигалка" в суперцикле.
2. В режиме SLEEP ядро останавливается, периферия продолжает работать.
3. Происходит инициализация NVIC и GPIO, переход в SLEEP происходит путем команды WFI (waint for interrupt). Выход из режима SLEEP возможен по настроенному фронту пина PA0-WKUP, который должен быть настроен через EXTI (External Interrupt Mode)и иметь разрешенное прерывание в NVIC. Вообще, ядро просыпается от срабатывания по триггеру на любой линии EXTI соответствующего пина. Алгоритм работы с вызовом Callback из предыдущей работы так же справедлив.
4. Перед входом в режим нужно остановить SysTick вызовом функции HAL_SuspendTick(), после выхода из режима возобновить через HAL_ResumeTick().
5. Чтобы отладчик в режиме SLEEP не отваливался нужно для IAR в Debugger->ST_LINK->Reset выбрать Connect during reset, для Keil в режиме отладки View->System Viewer->DBG открыть DBGMCU_CR, поставить галочку напротив DBG_SLEEP. 
Более подробно: https://www.st.com/resource/en/application_note/dm00354244-stm32-microcontroller-debug-toolbox-stmicroelectronics.pdf
6. Ввести ядро в режим сна можно так же командой WFE (waint for event).
7. Выход из режима SLEEP возможен по пину, настроенному и как External Interrupt Mode, так External Event Mode. Соотношение такое:
WFI - выход по Interrupt работает
WFE - выход по Interrupt работает
WFI - выход по Event НЕ работает
WFE - выход по Event работает