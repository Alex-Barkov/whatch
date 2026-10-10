#pragma once
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"

// === I2C (Общая шина: RTC, Touch, Audio, Expander) ===
#define WS_I2C_NUM          I2C_NUM_0
#define WS_I2C_SDA          GPIO_NUM_15
#define WS_I2C_SCL          GPIO_NUM_14
#define WS_I2C_CLK_SPEED    100000 // 100 кГц для максимальной стабильности

// === Display (CO5300 QSPI) ===
#define WS_LCD_SPI_NUM      SPI2_HOST
#define WS_LCD_PCLK         GPIO_NUM_11
#define WS_LCD_DATA0        GPIO_NUM_4
#define WS_LCD_DATA1        GPIO_NUM_5
#define WS_LCD_DATA2        GPIO_NUM_6
#define WS_LCD_DATA3        GPIO_NUM_7
#define WS_LCD_CS           GPIO_NUM_12
#define WS_LCD_RST          GPIO_NUM_NC // Управляется через экспандер или не требуется
#define WS_LCD_H_RES        466
#define WS_LCD_V_RES        466
#define WS_LCD_BPP          16

// === Touch (CST9217) ===
#define WS_TOUCH_INT        GPIO_NUM_16
#define WS_TOUCH_RST        GPIO_NUM_NC

// === SD Card (1-bit SDMMC) ===
#define WS_SD_CLK           GPIO_NUM_40
#define WS_SD_CMD           GPIO_NUM_39
#define WS_SD_D0            GPIO_NUM_38
#define WS_SD_MOUNT_POINT   "/sdcard"