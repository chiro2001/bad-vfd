#include "launcher.h"

#include <vector>
//#include "ili9488.h"
#include "app_remote_screen.h"
#include "lcd.h"
//#include "tft.h"
#include "ADF4351.h"
#include "vfd.h"

void StartLauncherTask() {
	static unsigned char parameter;
	xTaskHandle xHandle;
	// 创建任务，储存句柄
	xTaskCreate(LauncherTask, "LauncherTask", configMINIMAL_STACK_SIZE * 4,
			&parameter, osPriorityNormal, &xHandle);
	// 删除任务
	// if (xHandle != NULL)
	// 	vTaskDelete(xHandle);
	//	tft = new TFT(&hspi1, PinOut(TFT_RESET_GPIO_Port, TFT_RESET_Pin),
	//			PinOut(TFT_DC_GPIO_Port, TFT_DC_Pin),
	//			PinOut(TFT_LED_GPIO_Port, TFT_LED_Pin));
	//	tft->init();
}

FRESULT scan_files(char *path) {
	FRESULT res;
	DIR dir;
	UINT i;
	static FILINFO fno;

	res = f_opendir(&dir, path); /* Open the directory */
	if (res == FR_OK) {
		for (;;) {
			res = f_readdir(&dir, &fno); /* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0)
				break; /* Break on error or end of dir */
			if (fno.fattrib & AM_DIR) { /* It is a directory */
				i = strlen(path);
				sprintf(&path[i], "/%s", fno.fname);
				res = scan_files(path); /* Enter the directory */
				if (res != FR_OK)
					break;
				path[i] = 0;
			} else { /* It is a file. */
				printf("%s/%s\n", path, fno.fname);
			}
		}
		f_closedir(&dir);
	}

	return res;
}

void TestADF4351(bool once = false) {
	uint8_t buf[512];
	u8 line = 0;
	double f = 90.1;
	// 100kHz通道步进分辨率，MOD = F_in / step
	u32 step = 10000;  // (Hz)
	u32 F_in = 25;
	u16 MOD = (u16) (F_in * 1000000 / step);
	MOD = MOD > 4095 ? 4095 : MOD;
	u16 INT = 320;
	u16 FARC = 0;
	u16 R = 1;
	u8 div = RF_div32;
	u8 div_N = 1 << div;
//	auto done = PinOut(ADF4351_DONE_GPIO_Port, ADF4351_DONE_Pin);
//	done.reset();
	while (1) {
		line = 0;
		auto ad = ADF4351(&hspi2, PinOut(ADF4351_LE_GPIO_Port, ADF4351_LE_Pin),
				PinOut(ADF4351_CE_GPIO_Port, ADF4351_CE_Pin),
				PinOut(ADF4351_DONE_GPIO_Port, ADF4351_DONE_Pin));
		sprintf((char*) buf, "f: %.3lf", f);
		LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16, buf);
		line++;
//		done.set();
//		osDelay(10);
//		done.reset();
		INT = (u16) (f * div_N * 2 * R / F_in);
		FARC = (u16) (((f * R - (double) INT * F_in / (div_N * 2)) * (div_N * 2)
				/ F_in) * MOD);

		ad.set_freq(INT, FARC, MOD, div, R);
		sprintf((char*) buf, "I:%2d F:%5d", INT, FARC);
		LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16, buf);
		line++;
		sprintf((char*) buf, "D: %d, M: %d, R: %d", (int) div_N, MOD, (int) R);
		LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16, buf);
		line++;

		if (!once) {
			osDelay(30000);
			f += 0.1;
		} else {
			ST7735_LCD_Driver.FillRect(&st7735_pObj, 0, 0, ST7735Ctx.Width,
					ST7735Ctx.Height, BLACK);
			break;
		}
	}
}

void LauncherTask(void *argument) {
	uint8_t buf[512];
	uint8_t line = 0;
//	uint8_t res;
//	FILINFO fno;
//	DIR dir;
//	FIL f;
	//	static unsigned char parameter;
	//	xTaskHandle xHandle;
	//
	LCD_SetBrightness(0);
	ST7735_LCD_Driver.DrawBitmap(&st7735_pObj, 0, 0, (uint8_t*) gImage_head);
	LCD_Light(300, 300);
	LCD_Light(0, 300);
	ST7735_LCD_Driver.FillRect(&st7735_pObj, 0, 0, ST7735Ctx.Width,
			ST7735Ctx.Height, BLACK);
	LCD_Light(300, 100);
//	LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16,
//			(uint8_t*) "FATFS OK");
//	line++;
//	res = f_opendir(&dir, "0:/");
//	if (res != FR_OK) {
//		LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16,
//				(uint8_t*) "read dir error!");
//		while (true) {
//			osDelay(1000);
//		}
//	}
//	LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16,
//			(uint8_t*) "Read dir 0:/ OK");
//	line++;
//
//	for (;;) {
//		res = f_readdir(&dir, &fno);
//		if (res != FR_OK || fno.fname[0] == '\0') {
//			//			Error_Handler();
//			LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16,
//					(uint8_t*) "Read Done");
//			break;
//		}
//		sprintf((char*) buf, "Got item: %s", fno.fname);
//		LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16, buf);
//		line++;
//		if (line == 80 / 16 - 1)
//			line = 0;
//		sprintf((char*) buf, "0:/%s", fno.fname);
//		if (!(fno.fattrib & AM_DIR)) {
//			res = f_open(&f, (char*) buf, FA_READ);
//			if (res)
//				Error_Handler();
//			f_gets((char*) buf, sizeof(buf), &f);
//			LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16,
//					buf);
//			line++;
//			if (line == 80 / 16 - 1)
//				line = 0;
//		}
//		// osDelay(1000);
//	}

	TestADF4351(true);
	//	TestADF4351(false);
	auto e3 = PinOut(E3_GPIO_Port, E3_Pin);
	e3.set();
	osDelay(100);
	e3.reset();
	//	std::vector<uint16_t> colors {
	//	ILI9488_BLACK,
	//	ILI9488_RED,
	//	ILI9488_ORANGE,
	//	ILI9488_YELLOW,
	//	ILI9488_GREEN,
	//	ILI9488_CYAN,
	//	ILI9488_BLUE,
	//	ILI9488_PURPLE };

	HAL_TIM_Base_Start_IT(&htim2);

//	while (true) {
//		e3.set();
//		osDelay(100);
//		e3.reset();
//		osDelay(100);
//
//	}

	VFD_Init();

	osDelay(500);

	FIL f;
	FRESULT res;
	res = f_open(&f, "0:/ba.bin", FA_OPEN_EXISTING | FA_READ);
	if (res) {
		LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16,
				(u8*) "Cannot open file!");
		line++;
		while (1) {
			osDelay(1000);
		}
	}
	osDelay(500);

//	u32 fsize = f_size(&f);
//	u8 *data = new u8[sizeof(u8) * fsize + 1];
//	if (!data) {
//		LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16,
//				(u8*) "Cannot malloc!");
//		line++;
//
//		while (1)
//			osDelay(1000);
//	}
//	u8 *cnt = data;
//	UINT br;
//	f_read(&f, data, fsize, &br);

//	u8 disp[9] = "HELLO!  ";
//	while (1) {
//		VFD_DisplayString(0, disp);
//		u8 t = disp[0];
//		for (int i = 0; i < 7; i++) disp[i] = disp[i + 1];
//		disp[7] = t;
//		osDelay(300);
//	}

	u8 data[16];
	u16 frame_count = 0;
	u8 *cnt = data;
	UINT br;

	while (true) {
//		for (u8 i = 0; i < 8; i++) {
//			VFD_Send(0x40 + i);
//			for (u8 j = 0; j < 5; j++) {
//				VFD_Send(0xFF);
//			}
//		}
		res = f_read(&f, data, 15, &br);
		frame_count++;
		cnt = data;
		PT6312_CS_L();
		VFD_Send(0x40);
		for (u8 i = 0; i < 5 * 3; i++) {
			VFD_Send(*(cnt++));
		}
		PT6312_CS_H();
		VFD_DisplayRam(0, 3);
		osDelay(29);
		sprintf((char*) buf, "Frame %u", frame_count);
		LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16, buf);
		if (res || br != 15) {
			LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16,
					(u8*) "Done!");
			line++;

			break;
		}
	}

//	delete[] data;

	while (1) {
		osDelay(1000);
	}
}
