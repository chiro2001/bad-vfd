#include "ADF4351.h"

ADF4351 *adf4351 = nullptr;

void delay(u32 length) {
  int i;
  i = length * 200;
  while (i > 0) i--;
}

ADF4351::ADF4351(SPI_HandleTypeDef *SPI_, PinOut &&LE_, PinOut &&CE_,
                 PinOut &&DONE_)
    : SPI(SPI_), LE(LE_), CE(CE_), DONE(DONE_) {
	DONE.reset();
}

void ADF4351::write(u8 *dat, u8 size) {
  LE.reset();
  HAL_SPI_Transmit(SPI, dat, size, 1000);
  LE.set();
}

void ADF4351::write_reg(u32 reg) {
  u8 buf[4] = {0, 0, 0, 0};
  buf[0] = (u8)((reg >> 24) & (0X000000FF));
  buf[1] = (u8)((reg >> 16) & (0X000000FF));
  buf[2] = (u8)((reg >> 8) & (0X000000FF));
  buf[3] = (u8)((reg) & (0X000000FF));
  write(buf, 4);
}

void ADF4351::set_freq(u16 INT, u16 FARC, u16 MOD, u8 div, u16 R) {
  u32 adf_data;
  u16 adf_R = R;  // RF参考分频系数
  u8 adf_D = 0;   // RF REFin倍频器位(0 or 1)
  u8 adf_T = 0;   //参考二分频位,产生占空比50%,减少周跳
  u16 adf_Locktime = 4000;
  u8 RF_div = div;
  u16 adf_MOD = MOD;
  u16 adf_INT = INT;
  u16 adf_FARC = FARC;
  u16 adf_PHASE = 1;

  CE.reset();
  delay(1);
  CE.set();
  delay(2);

  LE.reset();
  delay(2);
  LE.set();

  DONE.set();

  //配置寄存器5
  adf_data = 0x00580000;  //数字锁存   LD引脚的工作方式为数字锁存   D23 D22=01
  adf_data = adf_data | ADF_R5;
  write_reg(adf_data);

  //配置寄存器4
  adf_data = 0x00800038;
  /*(DB23=1)The signal is taken from the VCO directly,信号直接从VCO获得
   可修改RF divider, R的值(DB22-20)the RF divider is 16;
   (DB11=0)VCO powerd up;	辅助RF输出禁用; 频段选择时钟,分频至125k,
   分频值160*/
  adf_data = adf_data | (RF_div << 20);  // RF divider
  adf_data = adf_data | (160 << 12);     //频段选择时钟
  adf_data = adf_data | ADF_R4;  //(DB5=1)RF output is enabled;(DB4-3=3H)Output
                                 // power level is 5dBm
  write_reg(adf_data);

  //配置寄存器3
  // 0    4    8    12   16   20   24   28
  // 0000 0000 1000 0100 1000 0000 0000 0000
  // 0000 0000 1000 0100 0000 0000 0000 0000
  // adf_data = 0x00848000;
  adf_data = 0x00848000;
  /*选择高频段（D23=1）, APB6ns(D22=0,=建议小数分频使用),,(D21=0,小数分频使用)
   使能CSR(D18=1),(D16 D15=01)快速锁定 可修改clock divider value的值*/
  adf_data = adf_data | (adf_Locktime << 3);
  adf_data = adf_data | ADF_R3;
  write_reg(adf_data);

  //配置寄存器2
  // 0    4    8    12   16   20   24   28
  // 0110 0001 0000 0100 0010 0000 1101 1000		MY
  // 0110 0001 0000 0000 0010 0000 0100 0000
  //   28   24   20   16   12    8    4    0
  //    |    |    |    |    |    |    |    |
  // 0111 1001 0000 0000 0010 0000 0100 0000
  // 启用参考倍频，二分频触发使能
  // 低噪声输出，MUXOUT输出N分频值
  // adf_data = 0x61002040;
  adf_data = 0x79002040;
  // adf_data = 0x610420C8;
  // //低杂散输出, 禁用参考倍频, 二分频触发器使能(减少周跳必须)
  //使能双缓冲器, 设置电荷磅电流0.31, 小数N分频(40), 设置R分频器的值为1
  //设置鉴相器的极性, (DB6)同向滤波器1,反向滤波器0,这里用同向滤波器
  adf_data = adf_data | (adf_D << 25);
  adf_data = adf_data | (adf_T << 24);
  adf_data = adf_data | (adf_R << 14);
  adf_data = adf_data | ADF_R2;
  write_reg(adf_data);

  //配置寄存器1
  adf_data = 0x01008000;
  //禁用相位调整,预分频器的值为8/9
  //相位字为1
  adf_data = adf_data | (adf_PHASE << 15);
  adf_data = adf_data | (adf_MOD << 3);
  adf_data = adf_data | ADF_R1;
  write_reg(adf_data);

  //配置寄存器0
  adf_data = 0x00000000;
  adf_data = adf_data | (adf_INT << 15);
  adf_data = adf_data | (adf_FARC << 3);
  adf_data = adf_data | ADF_R0;
  write_reg(adf_data);

  DONE.reset();
}

// Mhz
void ADF4351::freq(double f) {
  u32 step = 10000;  // (Hz)
  u32 F_in = 25;
  u16 MOD = (u16)(F_in * 1000000 / step);
  MOD = MOD > 4095 ? 4095 : MOD;
  u16 R = 1;
  u8 div = RF_div32;
  u8 div_N = 1 << div;
  u16 INT = (u16)(f * div_N * 2 * R / F_in);
  u16 FARC =
      (u16)(((f * R - (double)INT * F_in / (div_N * 2)) * (div_N * 2) / F_in) *
            MOD);
  set_freq(INT, FARC, MOD, div, R);

  // F_out = F_in * (INT + (FARC / MOD)) / div
}

void ADF4351_Init() {
  adf4351 = new ADF4351(&hspi2, PinOut(ADF4351_LE_GPIO_Port, ADF4351_LE_Pin),
                        PinOut(ADF4351_CE_GPIO_Port, ADF4351_CE_Pin),
                        PinOut(ADF4351_DONE_GPIO_Port, ADF4351_DONE_Pin));
}

void ADF4351_SetFreq(double freq) { adf4351->freq(freq); }
