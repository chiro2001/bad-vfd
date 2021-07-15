/*
 * vfd.h
 *
 *  Created on: Jul 15, 2021
 *      Author: Chiro
 */

#ifndef VFD_VFD_H_
#define VFD_VFD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "main.h"

/* 引脚定义，根据实际连接设置 */
/* 高压开关操作 */
#define HV_ON() HAL_GPIO_WritePin(VFD_HON_GPIO_Port, VFD_HON_Pin, GPIO_PIN_SET)
#define HV_OFF() \
  HAL_GPIO_WritePin(VFD_HON_GPIO_Port, VFD_HON_Pin, GPIO_PIN_RESET)
/* DA操作 */
#define PT6312_DA_H() \
  HAL_GPIO_WritePin(VFD_DA_GPIO_Port, VFD_DA_Pin, GPIO_PIN_SET)
#define PT6312_DA_L() \
  HAL_GPIO_WritePin(VFD_DA_GPIO_Port, VFD_DA_Pin, GPIO_PIN_RESET)
/* CP操作 */
#define PT6312_CP_H() \
  HAL_GPIO_WritePin(VFD_CP_GPIO_Port, VFD_CP_Pin, GPIO_PIN_SET)
#define PT6312_CP_L() \
  HAL_GPIO_WritePin(VFD_CP_GPIO_Port, VFD_CP_Pin, GPIO_PIN_RESET)
/* #CS操作 */
#define PT6312_CS_H() \
  HAL_GPIO_WritePin(VFD_CS_GPIO_Port, VFD_CS_Pin, GPIO_PIN_SET)
#define PT6312_CS_L() \
  HAL_GPIO_WritePin(VFD_CS_GPIO_Port, VFD_CS_Pin, GPIO_PIN_RESET)
/* #RST操作 */
#define PT6312_RST_H() \
  HAL_GPIO_WritePin(VFD_RST_GPIO_Port, VFD_RST_Pin, GPIO_PIN_SET)
#define PT6312_RST_L() \
  HAL_GPIO_WritePin(VFD_RST_GPIO_Port, VFD_RST_Pin, GPIO_PIN_RESET)

/**
 * @brief 向8MD06INKM发送数据
 *
 * @param u8Data 所要发送的数据
 */
void VFD_Send(uint8_t u8Data);

/**
 * @brief 8MD06INKM初始化
 *
 */
void VFD_Init(void);

/**
 * @brief 在8MD06INKM指定位置显示一个字符
 *
 * @param u8Position 位置，为0~8。
 * @param u8Char 要显示的字符，为ASCII编码。
 */
void VFD_DisplayChar(uint8_t u8Position, uint8_t u8Char);

/**
 * @brief 从指定位置开始显示字符串
 *
 * @param u8Position 指定开始显示的位置，为0~7。
 * @param Str 要显示的字符串指针。
 */
void VFD_DisplayString(uint8_t u8Position, uint8_t *Str);

void VFD_DisplayRam(uint8_t u8Position, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif /* VFD_VFD_H_ */
