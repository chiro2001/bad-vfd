/*
 * mainloop.c
 *
 *  Created on: Jul 9, 2021
 *      Author: Chiro
 */

#include "mainloop.h"
#include "GUI.h"
#include "test_gui.h"
#include "MainWindowDLG.h"

void Mainloop() {
//	static unsigned char parameter;
//	xTaskHandle xHandle;
//	xTaskCreate(TestGUI, "TestGUI", configMINIMAL_STACK_SIZE * 4, &parameter,
//			osPriorityNormal, &xHandle);
	WM_HWIN hwnd = test_chiui();  // CreateMainWindow();
	while (1) {
		GUI_TOUCH_Exec();
		GUI_TOUCH_Exec();
		GUI_Exec();
		GUI_Delay(10);
	}
}

void StartMainloop() {
	static unsigned char parameter;
	xTaskHandle xHandle;
	xTaskCreate(Mainloop, "Mainloop", configMINIMAL_STACK_SIZE * 4, &parameter,
			osPriorityNormal, &xHandle);
}

