#include "MainWindowDLG.h"

#include <stdio.h>

#include "ADF4351.h"
#include "DIALOG.h"
#include "cmsis_os.h"
#include "main.h"
#include "usart.h"
#include "test_chiui.h"

/*********************************************************************
 *
 *       Defines
 *
 **********************************************************************
 */
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x07)
#define ID_SLIDER_0 (GUI_ID_USER + 0x08)

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
 *
 *       Static data
 *
 **********************************************************************
 */

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
 *
 *       _aDialogCreate
 */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    {FRAMEWIN_CreateIndirect, "MainWindow", ID_FRAMEWIN_0, 0, 0, 480, 320, 0,
     0x0, 0},
    {SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 58, 214, 356, 20, 0, 0x0, 0},
    // USER START (Optionally insert additional widgets)
    // USER END
};

/*********************************************************************
 *
 *       Static code
 *
 **********************************************************************
 */

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
 *
 *       _cbDialog
 */
static void _cbDialog(WM_MESSAGE* pMsg) {
  WM_HWIN hItem;
  static char buf[64];
  int NCode;
  int Id;
  int minValue = 0, maxValue = 100, value = 0;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
    case WM_NOTIFY_PARENT:
      Id = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch (Id) {
        case ID_SLIDER_0:  // Notifications sent by 'Slider'
          switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
              // USER START (Optionally insert code for reacting on notification
//              test_chiui();
              // message) USER END
              break;
            case WM_NOTIFICATION_RELEASED:
              // USER START (Optionally insert code for reacting on notification
              // message) USER END
              break;
            case WM_NOTIFICATION_VALUE_CHANGED:
              // USER START (Optionally insert code for reacting on notification
              // message)
              hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
              SLIDER_GetRange(hItem, &minValue, &maxValue);
              value = SLIDER_GetValue(hItem);
              sprintf(buf, "[%d-%d] : %d\r\n", minValue, maxValue, value);
              double f = (double)value / (maxValue - minValue) * 20.0 + 90.0;
              // taskENTER_CRITICAL();
              // uprint(buf);
              ADF4351_SetFreq(f);
              // taskEXIT_CRITICAL();
              // USER END
              break;
              // USER START (Optionally insert additional code for further
              // notification handling) USER END
          }
          break;
          // USER START (Optionally insert additional code for further Ids)
          // USER END
      }
      break;
    // USER START (Optionally insert additional message handling)
    // USER END
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/*********************************************************************
 *
 *       Public code
 *
 **********************************************************************
 */
/*********************************************************************
 *
 *       CreateMainWindow
 */
WM_HWIN CreateMainWindow(void);
WM_HWIN CreateMainWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),
                             _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
