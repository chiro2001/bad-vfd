/*
 * ili9488.c
 *
 *  Created on: Jul 9, 2021
 *      Author: Chiro
 */
#include "ili9488.h"

void ili9488_write_data(uint8_t data) {
	HAL_GPIO_WritePin(ILI9488_DC_Port, ILI9488_DC_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1, &data, 1, 1000);
}

void ili9488_write_cmd(uint8_t d) {
	HAL_GPIO_WritePin(ILI9488_DC_Port, ILI9488_DC_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &d, 1, 1000);
}

void ili9488_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	ili9488_write_cmd(ILI9488_CASET);
	ili9488_write_data(x1 >> 8);
	ili9488_write_data(x1 & 0xFF);
	ili9488_write_data(x2 >> 8);
	ili9488_write_data(x2 & 0xFF);
	ili9488_write_cmd(ILI9488_PASET);
	ili9488_write_data(y1 >> 8);
	ili9488_write_data(y1 & 0xFF);
	ili9488_write_data(y2 >> 8);
	ili9488_write_data(y2 & 0xFF);
	ili9488_write_cmd(ILI9488_RAMWR);
}

void ili9488_push_color(uint16_t color) {
	uint8_t rgb[3];
	rgb[0] = (color & 0xF800) >> 11;
	rgb[1] = (color & 0x07E0) >> 5;
	rgb[2] = color & 0x001F;
	rgb[0] = ((uint16_t) rgb[0] * 255) / 31;
	rgb[1] = ((uint16_t) rgb[1] * 255) / 63;
	rgb[2] = ((uint16_t) rgb[2] * 255) / 31;
	HAL_SPI_Transmit(&hspi1, rgb, 3, 1000);
}

void ili9488_set_pixel(uint16_t x, uint16_t y, uint16_t color) {
	if ((x >= ILI9488_TFTWIDTH) || (y >= ILI9488_TFTHEIGHT))
		return;
	ili9488_set_window(x, y, x + 1, y + 1);
	HAL_GPIO_WritePin(ILI9488_DC_Port, ILI9488_DC_Pin, GPIO_PIN_SET);
	ili9488_push_color(color);
}

void ili9488_init() {
	HAL_GPIO_WritePin(ILI9488_RST_Port, ILI9488_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(ILI9488_RST_Port, ILI9488_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(ILI9488_LED_Port, ILI9488_LED_Pin, GPIO_PIN_SET);

	ili9488_write_cmd(0XF7);
	ili9488_write_data(0xA9);
	ili9488_write_data(0x51);
	ili9488_write_data(0x2C);
	ili9488_write_data(0x82);
	ili9488_write_cmd(0xC0);
	ili9488_write_data(0x11);
	ili9488_write_data(0x09);
	ili9488_write_cmd(0xC1);
	ili9488_write_data(0x41);
	ili9488_write_cmd(0XC5);
	ili9488_write_data(0x00);
	ili9488_write_data(0x0A);
	ili9488_write_data(0x80);
	ili9488_write_cmd(0xB1);
	ili9488_write_data(0xB0);
	ili9488_write_data(0x11);
	ili9488_write_cmd(0xB4);
	ili9488_write_data(0x02);
	ili9488_write_cmd(0xB6);
	ili9488_write_data(0x02);
	ili9488_write_data(0x42);
	ili9488_write_cmd(0xB7);
	ili9488_write_data(0xc6);
	ili9488_write_cmd(0xBE);
	ili9488_write_data(0x00);
	ili9488_write_data(0x04);
	ili9488_write_cmd(0xE9);
	ili9488_write_data(0x00);

	// ili9488_write_cmd(0x36);
	// ili9488_write_data((1 << 3) | (0 << 7) | (1 << 6) | (1 << 5));
	ili9488_direction(1);

	ili9488_write_cmd(0x3A);
	ili9488_write_data(0x66);
	ili9488_write_cmd(0xE0);
	ili9488_write_data(0x00);
	ili9488_write_data(0x07);
	ili9488_write_data(0x10);
	ili9488_write_data(0x09);
	ili9488_write_data(0x17);
	ili9488_write_data(0x0B);
	ili9488_write_data(0x41);
	ili9488_write_data(0x89);
	ili9488_write_data(0x4B);
	ili9488_write_data(0x0A);
	ili9488_write_data(0x0C);
	ili9488_write_data(0x0E);
	ili9488_write_data(0x18);
	ili9488_write_data(0x1B);
	ili9488_write_data(0x0F);
	ili9488_write_cmd(0XE1);
	ili9488_write_data(0x00);
	ili9488_write_data(0x17);
	ili9488_write_data(0x1A);
	ili9488_write_data(0x04);
	ili9488_write_data(0x0E);
	ili9488_write_data(0x06);
	ili9488_write_data(0x2F);
	ili9488_write_data(0x45);
	ili9488_write_data(0x43);
	ili9488_write_data(0x02);
	ili9488_write_data(0x0A);
	ili9488_write_data(0x09);
	ili9488_write_data(0x32);
	ili9488_write_data(0x36);
	ili9488_write_data(0x0F);
	ili9488_write_cmd(0x11);
	HAL_Delay(120);
	ili9488_write_cmd(0x29);
}

void ili9488_clear(uint16_t color) {
	ili9488_set_window(0, 0, ILI9488_TFTWIDTH - 1, ILI9488_TFTHEIGHT - 1);
	HAL_GPIO_WritePin(ILI9488_DC_Port, ILI9488_DC_Pin, GPIO_PIN_SET);
	for (uint32_t i = 0; i < ILI9488_TFTWIDTH * ILI9488_TFTHEIGHT; i++)
		ili9488_push_color(color);
}

void ili9488_draw(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
		uint8_t *dat) {
}

void ili9488_direction(uint8_t direction) {
	switch (direction) {
	case 0:
//		lcddev.width = LCD_W;
//		lcddev.height = LCD_H;
		ili9488_write_cmd(0x36); //BGR==1,MY==0,MX==0,MV==0
		ili9488_write_data((1 << 3) | (0 << 6) | (0 << 7));
		break;
	case 1:
//		lcddev.width = LCD_H;
//		lcddev.height = LCD_W;
		ili9488_write_cmd(0x36); //BGR==1,MY==1,MX==0,MV==1
		ili9488_write_data((1 << 3) | (0 << 7) | (1 << 6) | (1 << 5));
		break;
	case 2:
//		lcddev.width = LCD_W;
//		lcddev.height = LCD_H;
		ili9488_write_cmd(0x36); //BGR==1,MY==0,MX==0,MV==0
		ili9488_write_data((1 << 3) | (1 << 6) | (1 << 7));
		break;
	case 3:
//		lcddev.width = LCD_H;
//		lcddev.height = LCD_W;
		ili9488_write_cmd(0x36); //BGR==1,MY==1,MX==0,MV==1
		ili9488_write_data((1 << 3) | (1 << 7) | (1 << 5));
		break;
	default:
		break;
	}

}
