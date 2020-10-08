1. System Core -> WWDG -> Activated.
2. LED4 Output Push Pull.
3. WWDG -> Parameter Settings -> WWDG counter prescaler: 8, WWDG window value: 80, WWDG free-running downcounter value: 127.
4. WWDG находится на шине APB1 и тактируется частотой 84 МГц. Окно WWDG рассчитывается по формуле: Fapb1 * 4096 * WWDG_counter_prescaler * (WWDG_free-running_downcounter_value - WWDG_window_value (для максимального времени окна берется фиксированное значение 63 (0x3F))). Для текущего случая это будет: 
tmin = (1 / (84 * 10^6)) * 4096 * 8 * (127 - 80) = 18,3 мс;
tmax = (1 / (84 * 10^6)) * 4096 * 8 * (127 - 63) = 24,9 мс.
5. WWDG начинает считать со значения WWDG free-running downcounter вниз до 0x3F. Сбросить его, чтобы избежать сброса МК, можно в окне от значения WWDG window value до 0x3F. Во всех других случаях (достижение нижней границы WWDG 0x3F или сброс WWDG window value вне окна) будет происходить сброс микроконтроллера.
6. Из Reference Manual: Схема сторожевого таймера генерирует сброс MCU по истечении запрограммированного периода времени, если программа не обновляет содержимое счетчика вниз до того, как бит T6 станет 0. Сброс MCU также генерируется, если 7-битное значение счетчика вниз (в регистре управления) обновляется до того, как счетчик вниз достигает значения регистра окна. Это означает, что счетчик должен обновляться в ограниченном окне. Если сторожевой таймер активирован (бит WDGA установлен в регистре WWDG_CR) и когда 7-битный счетчик вниз (биты T [6: 0]) переключается с 0x40 на 0x3F (T6 очищается), он инициирует сброс. Если программное обеспечение перезагружает счетчик, когда счетчик больше, чем значение, хранящееся в регистре окна, то генерируется сброс.