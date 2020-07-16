/**
  ******************************************************************************
  * @file       : lis3dsh.c
  * @brief      : Файл с функциями для работы с акселерометром LIS3DSH
  * @details	: Содержит функции инициализации, записи/чтения, получение значений, их преобразование
  ******************************************************************************
  */
	
#include "lis3dsh.h"

SPI_HandleTypeDef LIS3DSH_spi;		///<Handle SPI для работы с LIS3DSH
GPIO_TypeDef *LIS3DSH_spi_cs_port;	///<Указатель на порт ножки CS
uint16_t LIS3DSH_spi_cs_pin;		///<Ножка CS
LIS3DSH_ERROR_t LIS3DSH_code_error;	///<Ошибка
double LIS3DSH_sensitivity = 0.0;	///<Чувствительность

uint8_t LIS3DSH_get_id(void);
void LIS3DSH_spi_cs(CS_STATE_t state);
void LIS3DSH_read(uint8_t *input_data, uint8_t read_address, uint16_t size_data);
void LIS3DSH_write(uint8_t *output_data, uint8_t write_address, uint16_t size_data);
void LIS3DSH_error_handler(LIS3DSH_ERROR_t LIS3DSH_error);
void LIS3DSH_spi_write_read(uint8_t *byte_tx, uint8_t *byte_rx);
double LIS3DSH_get_sensitivity(void);

/**
	* @brief 	Инициализация LIS3DSH
	* @details	Инициализация модуля SPI, ножки CS, установка кода отсутствия ошибки, чтение ID, 
	*		инициализация акселерометра следующими параметрами:
	*		-Скорость данных: 100 кГц
	*		-Разрешить оси X и Y
	*		-В зависимости от полученного значения чувствительности - выбор шкалы
	*		-SPI на 4 провода
	*		-Самотестирование отключено
	*		-Пропускная частота фильтра антидребезга - 800 Гц
	* @param	Handle SPI, указатель на порт ножки CS, ножка CS
	* @retval 	Нет
  */
void LIS3DSH_init(SPI_HandleTypeDef LIS3DSH_spi_for_init, GPIO_TypeDef *LIS3DSH_spi_cs_port_for_init, uint16_t LIS3DSH_spi_cs_pin_for_init) {
	uint8_t data_init = 0;
	
	LIS3DSH_spi 		= LIS3DSH_spi_for_init;
	LIS3DSH_spi_cs_port 	= LIS3DSH_spi_cs_port_for_init;
	LIS3DSH_spi_cs_pin 	= LIS3DSH_spi_cs_pin_for_init;
	LIS3DSH_code_error 	= LIS3DSH_ERROR_NONE;
	
	if (LIS3DSH_get_id() != LIS3DSH_ID) 
		LIS3DSH_error_handler(LIS3DSH_ERROR_ID);
	
	data_init = LIS3DSH_MASK_CTRL_REG4_ODR_100_HZ | LIS3DSH_MASK_CTRL_REG4_X_AXIS_ENABLE | LIS3DSH_MASK_CTRL_REG4_Y_AXIS_ENABLE;
	LIS3DSH_write(&data_init, LIS3DSH_ADDR_CTRL_REG4, 1);
	LIS3DSH_sensitivity = LIS3DSH_get_sensitivity();
	if (LIS3DSH_sensitivity == 0.06) 
		data_init = LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_2G;
	if (LIS3DSH_sensitivity == 0.12) 
		data_init = LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_4G;
	if (LIS3DSH_sensitivity == 0.18) 
		data_init = LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_6G;
	if (LIS3DSH_sensitivity == 0.24) 
		data_init = LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_8G;
	if (LIS3DSH_sensitivity == 0.73) 
		data_init = LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_16G;
	data_init |= LIS3DSH_MASK_CTRL_REG5_SPI_MODE_4WIRE | LIS3DSH_MASK_CTRL_REG5_SELFTEST_NORMAL | LIS3DSH_MASK_CTRL_REG5_FILTER_BANDWITH_800_HZ;
	LIS3DSH_write(&data_init, LIS3DSH_ADDR_CTRL_REG5, 1);
}

/**
	* @brief	Получение значения чувствительности
	* @details	Запрос значения чувствительности из REG5
	* @param	Нет
	* @retval 	Чувствительность в double
  */
double LIS3DSH_get_sensitivity(void) {
	double sensitivity 		= 0.0;
	uint8_t data_from_register 	= 0;
	
	LIS3DSH_read(&data_from_register, LIS3DSH_ADDR_CTRL_REG5, 1);
	switch (data_from_register & (LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_16G | LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_2G | \
			LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_4G | LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_6G | LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_8G)) {
		case LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_2G:
			sensitivity = 0.06;
			break;
		case LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_4G:
			sensitivity = 0.12;
			break;
		case LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_6G:
			sensitivity = 0.18;
			break;
		case LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_8G:
			sensitivity = 0.24;
			break;
		case LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_16G:
			sensitivity = 0.73;
			break;
	}
																
	return sensitivity;
}

/**
	* @brief	Получение значения по осям XYZ
	* @details	Читает младший и старший регистры со значениями для каждой оси
	* @param	Нет
	* @retval 	Структура strct_xyz с данными
  */
strct_xyz LIS3DSH_get_XYZ(void) {
	strct_xyz xyz;
	
	LIS3DSH_read(&xyz.x.low_val, 	LIS3DSH_ADDR_OUT_X_L, 1);
	LIS3DSH_read(&xyz.x.high_val,	LIS3DSH_ADDR_OUT_X_H, 1);
	LIS3DSH_read(&xyz.y.low_val, 	LIS3DSH_ADDR_OUT_Y_L, 1);
	LIS3DSH_read(&xyz.y.high_val, 	LIS3DSH_ADDR_OUT_Y_H, 1);
	LIS3DSH_read(&xyz.z.low_val, 	LIS3DSH_ADDR_OUT_Z_L, 1);
	LIS3DSH_read(&xyz.z.high_val, 	LIS3DSH_ADDR_OUT_Z_H, 1);
	
	return xyz;
}

/**
	* @brief	Получение значения по осям XYZ в виде int16
	* @details	Преобразует данные из структуры strct_xyz в strct_xyz_int_16, сдвигая регистры и умножая на
	* 		полученную чувствительность
	* @param	Нет
	* @retval 	Структура strct_xyz с данными в виде int16
  */
strct_xyz_int_16 LIS3DSH_get_XYZ_int16(void) {
	strct_xyz_int_16 xyz_16;
	strct_xyz xyz;
	
	xyz = LIS3DSH_get_XYZ();
	
	xyz_16.x = ((int16_t)(xyz.x.low_val | (xyz.x.high_val << 8))) * LIS3DSH_sensitivity;
	xyz_16.y = ((int16_t)(xyz.y.low_val | (xyz.y.high_val << 8))) * LIS3DSH_sensitivity;
	xyz_16.z = ((int16_t)(xyz.z.low_val | (xyz.z.high_val << 8))) * LIS3DSH_sensitivity;
	
	return xyz_16;
}

/**
	* @brief	Управление ножкой CS
	* @param	Состояние, которое нужно выставить
	* @retval 	Нет
  */
void LIS3DSH_spi_cs(CS_STATE_t state) {
	if (state == CS_PULL)
		HAL_GPIO_WritePin(LIS3DSH_spi_cs_port, LIS3DSH_spi_cs_pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LIS3DSH_spi_cs_port, LIS3DSH_spi_cs_pin, GPIO_PIN_RESET);
}

/**
	* @brief	Запись или чтение данных
	* @details	Отправка и прием данных через модуль SPI посредством функции HAL
	* @param	Байт для передачи, байт для приема
	* @retval 	Нет
  */
void LIS3DSH_spi_write_read(uint8_t *byte_tx, uint8_t *byte_rx) {
	if (HAL_SPI_TransmitReceive(&LIS3DSH_spi, byte_tx, byte_rx, 1, 100) != HAL_OK) {
		LIS3DSH_error_handler(LIS3DSH_ERROR_SPI);
	}
}

/**
	* @brief	Чтение из LIS3DSH
	* @details	Читает опрежеденное количество данных по определенному адресу
	* @param	Укузатель на данные, адрес, размер данных
	* @retval 	Нет
  */
void LIS3DSH_read(uint8_t *input_data, uint8_t read_address, uint16_t size_data) {
	uint8_t var_for_spi_clock = 0;
	
	read_address |= LIS3DSH_CMD_READ_WRITE;
	if (size_data > 1) read_address |= LIS3DSH_CMD_16_BIT;
	LIS3DSH_spi_cs(CS_DOWN);
	LIS3DSH_spi_write_read(&read_address, &var_for_spi_clock);	
	for (int i = 0; i < size_data; i++) {
		LIS3DSH_spi_write_read(&var_for_spi_clock, &input_data[i]);
	}
	LIS3DSH_spi_cs(CS_PULL);
}

/**
	* @brief	Запись в LIS3DSH
	* @details	Записывает опрежеденное количество данных по определенному адресу
	* @param	Укузатель на данные, адрес, размер данных
	* @retval 	Нет
  */
void LIS3DSH_write(uint8_t *output_data, uint8_t write_address, uint16_t size_data) {
	uint8_t var_for_spi_clock = 0;
	
	if (size_data > 1) write_address |= LIS3DSH_CMD_16_BIT;
	LIS3DSH_spi_cs(CS_DOWN);
	LIS3DSH_spi_write_read(&write_address, &var_for_spi_clock);
	for (int i = 0; i < size_data; i++) {
		LIS3DSH_spi_write_read(&output_data[i], &var_for_spi_clock);
	}
	LIS3DSH_spi_cs(CS_PULL);
}

/**
	* @brief	Получить ID
	* @param	Нет
	* @retval 	ID
  */
uint8_t LIS3DSH_get_id(void) {
	uint8_t id = 0;
	
	LIS3DSH_read(&id, LIS3DSH_ADDR_WHO_AM_I, 1);
	
	return id;
}

/**
	* @brief	Обработчик ошибки
	* @param	Ошибка
	* @retval 	Нет
  */
void LIS3DSH_error_handler(LIS3DSH_ERROR_t LIS3DSH_error) {
	LIS3DSH_code_error = LIS3DSH_error;
	while (LIS3DSH_code_error);
}
