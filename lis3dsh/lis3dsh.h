/**
  ******************************************************************************
	* @file		: lis3dsh.h
	* @brief	: Заголовочный файл с константами, описанием структур данных и прототипами функций
	*		  для работы с акселерометром LIS3DSH
	* @details	: Содержит функции инициализации, записи/чтения, получение значений, их преобразование
  ******************************************************************************
  */

#ifndef __LIS3DSH_H
#define __LIS3DSH_H

#include "main.h"
#include "spi.h"

#define LIS3DSH_ID	0x3F

#define LIS3DSH_ADDR_WHO_AM_I	0x0F
#define LIS3DSH_ADDR_OUT_X_L	0x28
#define LIS3DSH_ADDR_OUT_X_H	0x29
#define LIS3DSH_ADDR_OUT_Y_L	0x2A
#define LIS3DSH_ADDR_OUT_Y_H	0x2B
#define LIS3DSH_ADDR_OUT_Z_L	0x2C
#define LIS3DSH_ADDR_OUT_Z_H	0x2D
#define LIS3DSH_ADDR_CTRL_REG4	0x20
#define LIS3DSH_ADDR_CTRL_REG5	0x24

#define LIS3DSH_MASK_CTRL_REG4_X_AXIS_ENABLE		0x1
#define LIS3DSH_MASK_CTRL_REG4_Y_AXIS_ENABLE		0x2
#define LIS3DSH_MASK_CTRL_REG4_Z_AXIS_ENABLE		0x4
#define LIS3DSH_MASK_CTRL_REG4_XYZ_AXIS_ENABLE		0x7
#define LIS3DSH_MASK_CTRL_REG4_ODR_POWER_DOWN		0x0
#define LIS3DSH_MASK_CTRL_REG4_ODR_3_125_HZ		0x10
#define LIS3DSH_MASK_CTRL_REG4_ODR_6_15_HZ		0x20
#define LIS3DSH_MASK_CTRL_REG4_ODR_12_5_HZ		0x30
#define LIS3DSH_MASK_CTRL_REG4_ODR_25_HZ		0x40
#define LIS3DSH_MASK_CTRL_REG4_ODR_50_HZ		0x50
#define LIS3DSH_MASK_CTRL_REG4_ODR_100_HZ		0x60
#define LIS3DSH_MASK_CTRL_REG4_ODR_400_HZ		0x70
#define LIS3DSH_MASK_CTRL_REG4_ODR_800_HZ		0x80
#define LIS3DSH_MASK_CTRL_REG4_ODR_1600_HZ		0x90

#define LIS3DSH_MASK_CTRL_REG5_SPI_MODE_4WIRE		0x0
#define LIS3DSH_MASK_CTRL_REG5_SPI_MODE_3WIRE		0x1
#define LIS3DSH_MASK_CTRL_REG5_SELFTEST_NORMAL		0x0
#define LIS3DSH_MASK_CTRL_REG5_SELFTEST_POSITIVE_SIGN	0x2
#define LIS3DSH_MASK_CTRL_REG5_SELFTEST_NEGATIVE_SIGN	0x4
#define LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_2G	0x0
#define LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_4G	0x8
#define LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_6G	0x10
#define LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_8G	0x18
#define LIS3DSH_MASK_CTRL_REG5_FULLSCALE_SELECTION_16G	0x20
#define LIS3DSH_MASK_CTRL_REG5_FILTER_BANDWITH_800_HZ	0x0
#define LIS3DSH_MASK_CTRL_REG5_FILTER_BANDWITH_200_HZ	0x40
#define LIS3DSH_MASK_CTRL_REG5_FILTER_BANDWITH_400_HZ	0x80
#define LIS3DSH_MASK_CTRL_REG5_FILTER_BANDWITH_50_HZ	0xC0

#define LIS3DSH_CMD_READ_WRITE 	0x80
#define LIS3DSH_CMD_16_BIT	0x40			

///Управление ножной CS 
typedef enum {
	CS_PULL,		///<Ножку к питанию
	CS_DOWN			///<Ножку к нулю
} CS_STATE_t;

///Ошибки при работе
typedef enum {
	LIS3DSH_ERROR_NONE,	///<Нет ошибок
	LIS3DSH_ERROR_SPI,	///<Ошибка при отправке/приеме данных по SPI
	LIS3DSH_ERROR_ID	///<Некорректный ID
} LIS3DSH_ERROR_t;

///Значения из регистров данных по осям
typedef struct {
	uint8_t low_val;	///<Младший байт
	uint8_t high_val;	///<Старший байт
} axis;

///Значения по осям
typedef struct {
	axis x;		///<X
	axis y;		///<Y
	axis z;		///<Z
} strct_xyz;

///Значения по осям в int16
typedef struct {
	int16_t x;	///<X
	int16_t y;	///<Y
	int16_t z;	///<Z
} strct_xyz_int_16;

void LIS3DSH_init(SPI_HandleTypeDef LIS3DSH_spi_for_init, GPIO_TypeDef *LIS3DSH_spi_cs_port_for_init, uint16_t LIS3DSH_spi_cs_pin_for_init);
strct_xyz_int_16 LIS3DSH_get_XYZ_int16(void);

#endif /*__LIS3DSH_H */
