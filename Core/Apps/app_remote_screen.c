#include "app_remote_screen.h"
#include "remote_screen.h"

void RemoteScreenTask(void *argument) {
	uint8_t buf[512];
	uint8_t line = 0;

	// 测试：串口 Arduino 屏幕
	ST7735_LCD_Driver.FillRect(&st7735_pObj, 0, 0, ST7735Ctx.Width,
			ST7735Ctx.Height, BLACK);
	line = 0;
	union Combine com;
	com.clear.base.type = (u8) AF_CLEAR;
	com.clear.color = GREEN;
	SendFrame(&com, line);
	line++;
	sprintf((char*) buf, "Sent: %d", sizeof(Clear) + 2);
	LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16, buf);
	line++;

	while (1) {
		osDelay(1000);
	}
}
