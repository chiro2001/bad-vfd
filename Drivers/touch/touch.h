/*
 * touch.h
 *
 *  Created on: Jul 10, 2021
 *      Author: Chiro
 */

#ifndef TOUCH_TOUCH_H_
#define TOUCH_TOUCH_H_

#include "main.h"
#include "spi.h"

#define TP_PRES_DOWN 0x80  //触屏被按下
#define TP_CATH_PRES 0x40  //有按键按下了

//触摸屏控制器
typedef struct {
  u8 (*init)(void);      //初始化触摸屏控制器
  u8 (*scan)(u8);        //扫描触摸屏.0,屏幕扫描;1,物理坐标;
  void (*adjust)(void);  //触摸屏校准
  u16 x0;                //原始坐标(第一次按下时的坐标)
  u16 y0;
  u16 x;  //当前坐标(此次扫描时,触屏的坐标)
  u16 y;
  u8 sta;  //笔的状态
           // b7:按下1/松开0;
  // b6:0,没有按键按下;1,有按键按下.
  ////////////////////////触摸屏校准参数/////////////////////////
  float xfac;
  float yfac;
  short xoff;
  short yoff;
  //新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
  // touchtype=0的时候,适合左右为X坐标,上下为Y坐标的TP.
  // touchtype=1的时候,适合左右为Y坐标,上下为X坐标的TP.
  u8 touchtype;
  u8 adjusted;
} _m_tp_dev;

extern _m_tp_dev tp_dev;
extern u8 CMD_RDX;
extern u8 CMD_RDY;

#define PEN P_IN(T_IRQ_GPIO_Port, T_IRQ_Pin)
#define DOUT P_IN(T_DO_GPIO_Port, T_DO_Pin)
#define TDIN(v) P_OUT(T_DIN_GPIO_Port, T_DIN_Pin, v)
#define TCLK(v) P_OUT(T_CLK_GPIO_Port, T_CLK_Pin, v)
#define TCS(v) P_OUT(T_CS_GPIO_Port, T_CS_Pin, v)

void TP_Write_Byte(u8 num);      //向控制芯片写入一个数据
u16 TP_Read_AD(u8 CMD);          //读取AD转换值
u16 TP_Read_XOY(u8 xy);          //带滤波的坐标读取(X/Y)
u8 TP_Read_XY(u16 *x, u16 *y);   //双方向读取(X+Y)
u8 TP_Read_XY2(u16 *x, u16 *y);  //带加强滤波的双方向坐标读取
void TP_Drow_Touch_Point(u16 x, u16 y, u32 color);  //画一个坐标校准点
void TP_Draw_Big_Point(u16 x, u16 y, u32 color);    //画一个大点
u8 TP_Scan(u8 tp);                                  //扫描
void TP_Save_Adjdata(void);                         //保存校准参数
u8 TP_Get_Adjdata(void);                            //读取校准参数
void TP_Adjust(void);                               //触摸屏校准
u8 TP_Init(void);                                   //初始化

#endif /* TOUCH_TOUCH_H_ */
