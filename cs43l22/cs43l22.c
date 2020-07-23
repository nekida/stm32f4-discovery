/**
  ******************************************************************************
  * @file			cs43l22.c
  * @brief    Файл с функциями для работы с цифро-аналоговым преобразователем CS43L22
  ******************************************************************************
  */
	
#include "cs43l22.h"

I2C_HandleTypeDef 	CS43L22_i2c;			///<Handle I2C для работы с CS43L22
I2S_HandleTypeDef	CS43L22_i2s;			///<Handle I2S для работы с CS43L22
GPIO_TypeDef		*CS43L22_reset_port;	///<Указатель на порт ножки Reset
uint16_t 			LIS3DSH_reset_pin;		///<Ножка Reset
CS43L22_ERROR_t 	CS43L22_code_error;		///<Ошибка
chip 				CS43L22;				///<Уникальный номер  

void 		CS43L22_error_handler(CS43L22_ERROR_t CS43L22_error);
uint8_t 	CS43L22_i2c_read(CS43L22_REG_t read_address);
void 		CS43L22_i2c_write(CS43L22_REG_t write_address, uint8_t output_data);
chip 		CS43L22_get_id(void);
void 		CS43L22_reset_set(CS43L22_RESET_SET_t state);

/**
	* @brief	Необходимая инициализация из Datasheet'а
	* @param	Нет
	* @retval 	Нет
  */
void CS43L22_required_initialization_settings(void) {
	CS43L22_i2c_write((CS43L22_REG_t)0x00, 0x99);
	CS43L22_i2c_write((CS43L22_REG_t)0x47, 0x80);
	CS43L22_i2c_write((CS43L22_REG_t)0x32, (1 << 7));
	CS43L22_i2c_write((CS43L22_REG_t)0x32, (0 << 7));
	CS43L22_i2c_write((CS43L22_REG_t)0x00, 0x00);
}

/**
	* @brief 	Включение беззвучного режима
	* @param	-CS43L22_MUTE_ON	- включить беззвучный режим
	* \param	-CS43L22_MUTE_OFF 	- выключить беззвучный режим (возвращение к заданной громкости из функции CS43L22_init)
	* @retval 	Нет
  */
void CS43L22_mute(bool state) {
	if (state == CS43L22_MUTE_ON) {
		CS43L22_i2c_write(CS43L22_REG_ADDR_VOLUME_PCMA, CS43L22_REG_VOLUME_PCM_MUTE_EN);
		CS43L22_i2c_write(CS43L22_REG_ADDR_VOLUME_PCMB, CS43L22_REG_VOLUME_PCM_MUTE_EN);
	} else {
		CS43L22_i2c_write(CS43L22_REG_ADDR_VOLUME_PCMA,	CS43L22_REG_VOLUME_PCM_PLUS_12DB);
		CS43L22_i2c_write(CS43L22_REG_ADDR_VOLUME_PCMB,	CS43L22_REG_VOLUME_PCM_PLUS_12DB);
	}
}

/**
	* @brief 	Инициализация CS43L22
	* @details	Инициализация модуля I2C, модуля I2S, ножки Reset, установка кода отсутствия ошибки, выставление Reset в 1, чтение ID, рекомендуемые настройки из datasheet'a, 
	*			инициализация ЦАП следующими параметрами:
	* \details	Выключить питание
	* \details	-Режим аудиосигнала
	* \details 	--Отключение микрофона и включение наушников
	* \details 	--Автоопределение скорости и деление MCLK на 2 (96 / 2 = 48, так как выбрана частота звука 48 кГц
	* \details 	--I2S до 24 бит данных
	* \details 	--Исходный уровень громкости +12 дБ по обоим каналам
	* \details 	--Постепенное увеличение громкости от текущего уровня до нового уровня с указанной скоростью (1/2 дБ каждые 16 циклов частоты выбора аквтивного канала LRCK)
	* \details 	--Включить 15/50 мкс реагирование фильтра на компенсацию искажений
	* \details 	--Выключить ограничитель
	* \details 	--Бас на -10,5 дБ
	* \details 	--Установить уровень громкости +12 дБ по обоим каналам
	* \details  -Режим аналогового сигнала
	* \details 	--I2S до 24 бит данных
	* \details	Включить питание
	* @param	Handle I2C, handle I2S, указатель на порт ножки Reset, ножка Reset, назначения ЦАП
	* @retval 	Нет
  */
void CS43L22_init(I2C_HandleTypeDef CS43L22_i2c_for_init, I2S_HandleTypeDef CS43L22_i2s_for_init, GPIO_TypeDef* CS43L22_reset_port_for_init, uint16_t CS43L22_reset_pin_for_init, CS43L22_PURPOSE_t purpose) {
	CS43L22_i2c 		= CS43L22_i2c_for_init;
	CS43L22_i2s 		= CS43L22_i2s_for_init;
	CS43L22_reset_port	= CS43L22_reset_port_for_init;
	LIS3DSH_reset_pin  	= CS43L22_reset_pin_for_init;
	CS43L22_code_error 	= CS43L22_ERROR_NONE;
	
	CS43L22_reset_set(CS43L22_SET);
	
	CS43L22 = CS43L22_get_id();
	CS43L22_required_initialization_settings();
				
	CS43L22_i2c_write(CS43L22_REG_ADDR_PWR_CNTRL_1,	CS43L22_REG_PWR_CNTRL_1_WR_POWERED_DOWN);
	if (purpose == CS43L22_AUDIO_SIGNAL) {
		CS43L22_i2c_write(CS43L22_REG_ADDR_PWR_CNTRL_2, 			CS43L22_REG_PWR_CNTRL_2_WR_HEADPHONE_A_B_ON | CS43L22_REG_PWR_CNTRL_2_WR_SPEAKER_A_B_OFF);
		CS43L22_i2c_write(CS43L22_REG_ADDR_CLK_CNTRL, 				CS43L22_REG_CLK_CNTRL_WR_AUTO_DETECT_SPEED | CS43L22_REG_CLK_CNTRL_WR_MCLK_DIVIDED_BY_2);
		CS43L22_i2c_write(CS43L22_REG_ADDR_IFACE_CNTRL_1, 			CS43L22_REG_IFACE_CNTRL_1_WR_DAC_IFACE_FORMAT_I2S_UP_24B);
		CS43L22_i2c_write(CS43L22_REG_ADDR_MASTER_VOLUME_CNTRL_A,	CS43L22_REG_MASTER_VOLUME_CNTRL_WR_PLUS_12DB);
		CS43L22_i2c_write(CS43L22_REG_ADDR_MASTER_VOLUME_CNTRL_B,	CS43L22_REG_MASTER_VOLUME_CNTRL_WR_PLUS_12DB);	
		CS43L22_i2c_write(CS43L22_REG_ADDR_ANALOG_ZC_SR_SETTINGS,	CS43L22_REG_ANALOG_ZC_SR_SETTINGS_WR_NOT_OCCUR_SOFT_RAMP | CS43L22_REG_ANALOG_ZC_SR_SETTINGS_WR_NOT_OCCUR_ZERO_CROSS);
		CS43L22_i2c_write(CS43L22_REG_ADDR_MISCELLANEOUS_CNTRLS,	CS43L22_REG_MISCELLANEOUS_CNTRLS_WR_DEEMPHASIS_EN);
		CS43L22_i2c_write(CS43L22_REG_ADDR_LIMITER_CNTRL_1,			CS43L22_REG_LIMITER_CNTRL_1_LIMITER_OFF);
		CS43L22_i2c_write(CS43L22_REG_ADDR_TONE_CNTRL,				CS43L22_REG_TONE_CNTRL_BASS_GAIN_MINUS_10_5DB);
		CS43L22_i2c_write(CS43L22_REG_ADDR_VOLUME_PCMA,				CS43L22_REG_VOLUME_PCM_PLUS_12DB);
		CS43L22_i2c_write(CS43L22_REG_ADDR_VOLUME_PCMB,				CS43L22_REG_VOLUME_PCM_PLUS_12DB);
	} else {
		CS43L22_i2c_write(CS43L22_REG_ADDR_IFACE_CNTRL_1, CS43L22_REG_IFACE_CNTRL_1_WR_DAC_IFACE_FORMAT_I2S_UP_24B);
	}
	CS43L22_i2c_write(CS43L22_REG_ADDR_PWR_CNTRL_1,	CS43L22_REG_PWR_CNTRL_1_WR_POWERED_UP);
}

/**
	* @brief	Управление ножкой Reset
	* @param	Состояние, которое нужно выставить
	* @retval 	Нет
  */
void CS43L22_reset_set(CS43L22_RESET_SET_t state) {
	if (state == CS43L22_RESET)	
		HAL_GPIO_WritePin(CS43L22_reset_port, LIS3DSH_reset_pin, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(CS43L22_reset_port, LIS3DSH_reset_pin, GPIO_PIN_SET);
}

/**
	* @brief	Получить ID
	* @param	Нет
	* @retval 	ID
  */
chip CS43L22_get_id(void) {
	chip data_chip;
	uint8_t id_rev = 0;
	
	id_rev = (CS43L22_i2c_read(CS43L22_REG_RO_ADDR_CHIP_ID));
	data_chip.id	= (id_rev >> 3);
	data_chip.rev 	= (id_rev & 0x7);
	
	return data_chip;
}

/**
	* @brief	Прочитать данные по адресу
	* @param	Адрес данных
	* @retval 	Данные
  */
uint8_t CS43L22_i2c_read(CS43L22_REG_t read_address) {
	uint8_t input_data = 0;
	
	if (HAL_I2C_Master_Transmit(&CS43L22_i2c, (uint16_t)CS43L22_DEV_ADDR, (uint8_t *)&read_address, 1, 100) != HAL_OK)
		CS43L22_error_handler(CS43L22_ERROR_I2C_READ_TX);
	
	if (HAL_I2C_Master_Receive(&CS43L22_i2c, (uint16_t)CS43L22_DEV_ADDR, &input_data, 1, 100) != HAL_OK)
		CS43L22_error_handler(CS43L22_ERROR_I2C_READ_RX);
	
	return input_data;
}

/**
	* @brief	Записать данные по адресу
	* @param	Адрес данных, данные
	* @retval 	Нет
  */
void CS43L22_i2c_write(CS43L22_REG_t write_address, uint8_t output_data) {
	uint8_t tx_data[2] = {0};
	
	tx_data[0] = (uint8_t)write_address;
	tx_data[1] = output_data;
	
	if (HAL_I2C_Master_Transmit(&CS43L22_i2c, (uint16_t)CS43L22_DEV_ADDR, tx_data, 2, 100) != HAL_OK)
		CS43L22_error_handler(CS43L22_ERROR_I2C_WRITE_TX);
}

/**
	* @brief 	Обработчик ошибки
	* @param	Ошибка
	* @retval 	Нет
  */
void CS43L22_error_handler(CS43L22_ERROR_t CS43L22_error) {
	CS43L22_code_error = CS43L22_error;
	while(CS43L22_code_error);
}
