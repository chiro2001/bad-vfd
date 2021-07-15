/*
 * ADF4351.h
 *
 *  Created on: 2021年7月6日
 *      Author: Chiro
 */

#ifndef CHIEMLIB_ADF4351_H_
#define CHIEMLIB_ADF4351_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pin.h"
#include "main.h"
#include "spi.h"

//#define
#define ADF4351_R0			((u32)0X2C8018)
#define ADF4351_R1			((u32)0X8029)
#define ADF4351_R2			((u32)0X10E42)
#define ADF4351_R3			((u32)0X4B3)
#define ADF4351_R4			((u32)0XEC803C)
#define ADF4351_R5			((u32)0X580005)

#define ADF4351_R1_Base	((u32)0X8001)
#define ADF4351_R4_Base	((u32)0X8C803C)
#define ADF4351_R4_ON	  ((u32)0X8C803C)
#define ADF4351_R4_OFF	((u32)0X8C883C)

#define ADF4351_RF_OFF	((u32)0XEC801C)

#define ADF4351_PD_ON		((u32)0X10E42)
#define ADF4351_PD_OFF	((u32)0X10E02)

#define ADF_R0 0
#define ADF_R1 1
#define ADF_R2 2
#define ADF_R3 3
#define ADF_R4 4
#define ADF_R5 5
#define RF_div1 0
#define RF_div2 1
#define RF_div4 2
#define RF_div8 3
#define RF_div16 4
#define RF_div32 5
#define RF_div64 6

#ifdef __cplusplus

class ADF4351 {
public:
	SPI_HandleTypeDef *SPI = nullptr;
	PinOut LE;
	PinOut CE;
	PinOut DONE;
	ADF4351(SPI_HandleTypeDef *SPI_, PinOut &&LE_, PinOut &&CE_, PinOut &&DONE_);
	void write(u8 *dat, u8 size);
	void write_reg(u32 reg);
	void freq(double f);
	void set_freq(u16 INT, u16 FARC, u16 MOD, u8 div, u16 R);
};

extern ADF4351 *adf4351;

#endif

void ADF4351_Init();
void ADF4351_SetFreq(double freq);

#ifdef __cplusplus
}
#endif

#endif /* CHIEMLIB_ADF4351_H_ */
