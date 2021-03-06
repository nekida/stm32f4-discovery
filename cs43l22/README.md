# Цифро-аналоговый преобразователь CS43L22

## Описание конфигурации проекта
Тактирование от внешнего генератора 8 МГц.
Системные клоки получаются через PLL и составляют 168 МГц.
Все шины тактируются максимальной для них частотой.
I2S тактируется PLLI2SCLK частотой 96 МГц.

Схема подключения:

![Image alt](https://github.com/nekida/stm32f4-discovery/blob/master/cs43l22/pic/schematic.jpg)

Используется I2C1. Его настройка:

![Image alt](https://github.com/nekida/stm32f4-discovery/blob/master/cs43l22/pic/I2C_config.jpg)

Используется I2S3. Его настройка:

![Image alt](https://github.com/nekida/stm32f4-discovery/blob/master/cs43l22/pic/I2S_config.jpg)

Настройка ЦАП производится путем записи значений в регистры по интерфейсу I2C. Данные управления выходным аналоговым сигналом передаются по I2S.

## Описание ЦАП и основное из datasheet'a

CS43L22 - это 24 битный ЦАП с коэффициентом передескретизации 128 * частоту дескретизации системы, поддерживающий дельта-сигма модуляцию. Частота шим модулятора = 384 кГц.
Цифровое представление аналового сигнала (Pulse Code Modulation, PCM, Импульсно-кодовая модуляция) поступает на движок DSP через интерфейс SAI (Serial Audio Interface), реализованный на I2S. Внутри движка после фильтра верхних частот поступает на блок восстановления, где можно включить восстановление появившихся искажений. Далее идет блок громкости, где можно задать значение громкости для каждого канала, включить беззвучный режим и режим инвертирования. Следующий блок - блок переключения между каналами, где помимо выбора основного канала, можно включить (left + right) / 2 микширование. Сумматор может сложить входной сигнал с сигналом определенной октавы, который генерирует Beep генератор. После сумматора идет блок управления основной громкости сигнала, где так же можно включить беззвучный режим, плавное нарастание, групповое управление. В следующем блоке настраивается характеристика выходного сигнала. Сам сигнал можно пропустить через ограничитель, который понизит общую громкость при превышении выходным сигналом определенного уровня, и снова подать на блок управления основной громкостью. Так же, полученный сигнал пойдет на Многоразрядный дельта-сигма ЦАП, который и выдаст сигнал на аудиоразъем на плате; так же этот сигнал идет на ШИМ модулятор, который путем управления полевыми транзисторами может генерировать выходной сигнал микрофона, переключать режим Стерео/Моно.

## Описание библиотеки

Подключите к проекту файл cs43l22.h. В зависимости от целей - проделайте шаги из раздела с описанием примеров. Для того, чтобы сконфигурировать ЦАП, вызовите функцию CS43L22_init() и передайте ей следующие параметры: I2C_HandleTypeDef выбранного модуля I2C, выбранного модуля I2S, GPIO_TypeDef* и uint16_t ножки, которая будет Reset, CS43L22_PURPOSE_t режима, в котором хотите работать (один подходит для генерации синусоиды определенной частоты, другой - для генерации звука).

## Описание примеров
### audio_example.c

Этот пример воспроизводит 7 нот первой октавы. Для прослушивания потребуются наушники или колонки. Для работы потребуются файлы: notes.с и notes.h с функциями настройки таймера для каждой ноты.


Для работы этого примера потребуется включить таймер TIM1:

![Image alt](https://github.com/nekida/stm32f4-discovery/blob/master/cs43l22/pic/TIM1_config.jpg)

Настраивая его на прерывания с разной частотой получаем меандр разной частоты. Настраивая таймер метотодом подбора так, чтобы добиться определенной частоты, соответсвующей определенной ноте. Настройка каждого значения нот происходит в функции. Был создан массив таких функций, находящийся в начале файла примера. Время, которое звучит каждая нота, определяется переменной counter_note, которая инициализируется значением на единицу меньшим, чем значение, совпадение с которым приведет к смене ноты. Это сделано для того, чтобы при первом проходе попасть на нулевой элемент массива функций, где находится функция для ноты До.
Функция tim2_switch_val(), в которой происходит инвертирование флага значения данных для отправки по I2S, вызывается в обработчике прерывания таймера TIM1_UP_TIM10_IRQHandler().

### signal_example.c

Этот пример генерирует синусоиду на выходе определенной частоты. Потребуется осциллограф. Для работы потребуются файлы: analog_signals.с и analog_signals.h с функциями создания синусоиды и посылкой ее параметров по I2S.

Вызываете функцию analog_signal_create() с аргументом значения частоты (примерно, от 10 Гц до 20 кГц), в бесконечном цикле вызываете функцию analog_signal_generate(). 
