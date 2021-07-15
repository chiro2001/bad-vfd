/*
 * remote_screen.h
 *
 *  Created on: 2021年7月2日
 *      Author: Chiro
 */

#ifndef INC_REMOTE_SCREEN_H_
#define INC_REMOTE_SCREEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "lcd.h"

//#define BLACK 0x0000
//#define BLUE 0x001F
//#define RED 0xF800
//#define GREEN 0x07E0
//#define CYAN 0x07FF
//#define MAGENTA 0xF81F
//#define YELLOW 0xFFE0
//#define WHITE 0xFFFF

#ifndef u8
typedef uint8_t u8;
#endif

#ifndef u16
typedef uint16_t u16;
#endif

#ifndef u32
typedef uint32_t u32;
#endif

enum AF_TYPE {
	AF_IMAGE = 1, AF_TEXT, AF_FILL, AF_CLEAR, AF_LINE,
};

// 传输结构体
#pragma pack(1)
typedef struct {
	u8 type;
} Base;

#pragma pack(1)
typedef struct {
	Base base;
	u16 x1;
	u16 y1;
	u16 x2;
	u16 y2;
	u32 length;
// 直接写入屏幕
// u8 data[];
} Image;

#pragma pack(1)
typedef struct {
	Base base;
	u16 x1;
	u16 y1;
	u16 x2;
	u16 y2;
	u8 size;
	u16 color;
} Line;

#pragma pack(1)
typedef struct {
	Base base;
	u16 x1;
	u16 y1;
	u16 x2;
	u16 y2;
	u16 color;
} Fill;

#pragma pack(1)
typedef struct {
	Base base;
	u16 color;
} Clear;

#pragma pack(1)
typedef struct {
	Base base;
	u16 x;
	u16 y;
	u8 size;
	u16 color;
	u32 length;
// 数据不包括 \0...?
// u8 data[];
} Text;

// #pragma pack(1)
union Combine {
	Base base;
	Image image;
	Text text;
	Clear clear;
	Fill fill;
	Line line;
};

void SendFrame(union Combine *com, u16 line);


#endif /* INC_REMOTE_SCREEN_H_ */
