#include "remote_screen.h"

u8 frame_start = 's';
u8 frame_end = 'e';

void SendFrame(union Combine *com, u16 line) {
	char buf[64];
	char res[512];
	u8 tmp[sizeof(union Combine) + 2];
	if (!com)
		return;
	sprintf(res, "%02X", frame_start);
	HAL_UART_Transmit(&huart2, &frame_start, 1, 0xffff);
	memcpy(tmp, com, sizeof(union Combine));
	switch (com->base.type) {
	case AF_CLEAR:
		// HAL_UART_Transmit(&huart2, (u8*) &com, sizeof(Clear), 0xffff);
		HAL_UART_Transmit(&huart2, tmp, sizeof(Clear), 0xffff);
		for (u16 i = 0; i < sizeof(Clear); i++) {
			// sprintf((char*) buf, "%s %02X", buf, ((u8*) &com)[i]);
			sprintf(buf, " %02X", (u16) tmp[i]);
			strcat(res, buf);
		}
		break;
	default:
		sprintf(buf, " [ERR]");
		strcat(res, buf);
		break;
	}
	sprintf(buf, " %02X", (u16) frame_end);
	strcat(res, buf);
	HAL_UART_Transmit(&huart2, &frame_end, 1, 0xffff);
	LCD_ShowString(0, line * 16, ST7735Ctx.Width, (line + 1) * 16, 16,
			(uint8_t*) buf);
}

