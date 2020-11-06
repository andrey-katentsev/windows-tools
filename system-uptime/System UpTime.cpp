#include "Resource.h"

#include <windows.h>
#include <windowsx.h>

#include <tchar.h>
#include <strsafe.h>

#include "hsSysTray.h"

#define HS_MESSAGE_MAX 128
#define HS_MESSAGE_FORMAT "%.2hu:%.2hu:%.2hu:%.2hu"

#define HS_TIMER_ID 1
#define HS_TIMER_TIMEOUT 1000

#define HS_MS_TO_SECONDS  1000
#define HS_MS_TO_MINUTES 60000
#define HS_MS_TO_HOURS 3600000
#define HS_MS_TO_DAYS 86400000

#define HS_ICON_ID 1
#define HS_SYSTRAY_TIP_FORMAT "¬рем€ работы системы %.2hu:%.2hu:%.2hu:%.2hu"

namespace // internal processing
{
	class CState
	{
	public:
		// DEFECT: KAA: может быть запущено скрытым (нужно анализировать значение nShowCommand полученное в wWinMain).
		CState() : m_isVisible(true) {}

		void SetVisible(bool isVisible)
		{ m_isVisible = isVisible; }

		bool IsVisible(void)
		{ return m_isVisible; }

	private:
		bool m_isVisible;
	};

	CState* GetWindowState(void)
	{
		static CState window_state;
		return &window_state;
	}

	inline BOOL Activate(HWND hWnd)
	{
		GetWindowState()->SetVisible(true);
		return AnimateWindow(hWnd, 350, AW_ACTIVATE | AW_BLEND);
	}

	inline BOOL Minimize(HWND hWnd)
	{
		GetWindowState()->SetVisible(false);
		SetProcessWorkingSetSize(GetCurrentProcess(), static_cast<SIZE_T>(-1), static_cast<SIZE_T>(-1));
		return AnimateWindow(hWnd, 400, AW_HIDE | AW_BLEND);
     //AnimateWindow(hDlg,500,AW_HIDE|AW_HOR_POSITIVE|AW_SLIDE);
     //ShowWindow(hDlg,SW_MINIMIZE);
     //ShowWindow(hDlg,SW_HIDE);
	}

	inline BOOL Close(HWND hWnd)
	{
		GetWindowState()->SetVisible(false);
		return AnimateWindow(hWnd, 250, AW_HIDE | AW_BLEND);
	}

	BOOL OnClose(HWND hWnd)
	{
		Close(hWnd);
		hsSystemTrayRemove(hWnd, HS_ICON_ID);
		KillTimer(hWnd, HS_TIMER_ID);
		EndDialog(hWnd, 0);
		return TRUE;
	}
} // internal processing

//  ак правило, процедура диалогового окна возвращает значение TRUE, если она обработала сообщение, и FALSE в противном случае.
LRESULT CALLBACK SystemUpTime(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL isProcessed = FALSE;
 switch(uMsg)
 {
 case WM_INITDIALOG:
  {
   // «агрузка изображени€.
   HANDLE hIcon=LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_SHELL32_013), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
   // ”становка иконки диалогового окна.
   SendMessage(hDlg,WM_SETICON,ICON_BIG,(LPARAM)hIcon);
   SendMessage(hDlg,WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
   // ”становка таймера.
   SetTimer(hDlg,HS_TIMER_ID,HS_TIMER_TIMEOUT,NULL);
   // ќтображение иконки в системной области.
   hsSystemTrayAdd(hDlg,HS_ICON_ID,(HICON)hIcon,TEXT("»нициализаци€."));
   return TRUE;
  } break;
 case WM_TIMER:
  {
   switch(wParam)
   {
   case HS_TIMER_ID:
    {
     DWORD dwTime=GetTickCount();
     SYSTEMTIME ST;
     TCHAR szMessage[HS_MESSAGE_MAX];
     HWND hCSBT=GetDlgItem(hDlg,IDC_SYSTEM_UPTIME_STATIC);

     ST.wSecond = (dwTime/HS_MS_TO_SECONDS)%60;
     ST.wMinute = (dwTime/HS_MS_TO_MINUTES)%60;
     ST.wHour = (dwTime/HS_MS_TO_HOURS)%24;
     ST.wDay = (dwTime/HS_MS_TO_DAYS)%365;

     //ST.wSecond=(dwTime/1000)%60;
     //ST.wMinute=((dwTime/1000)/60)%60;
     //ST.wHour=(((dwTime/1000)/60)/60)%24;
     //ST.wDay=(WORD)(((dwTime/1000)/60)/60)/24;

     _stprintf_s(szMessage,HS_MESSAGE_MAX,TEXT(HS_MESSAGE_FORMAT),ST.wDay,ST.wHour,ST.wMinute,ST.wSecond);
     SendMessage(hCSBT,WM_SETTEXT,NULL,(LPARAM)szMessage);

     // ќбновление иконки в системной области.
     StringCchPrintf(szMessage,HS_MESSAGE_MAX,TEXT(HS_SYSTRAY_TIP_FORMAT),ST.wDay,ST.wHour,ST.wMinute,ST.wSecond);
     hsSystemTrayModify(hDlg,HS_ICON_ID,NULL,szMessage);

     // ѕриложение должно возвратить 0, если обработало сообщение WM_TIMER.
     return 0;
    } break;
   }
   return 1;
  } break;
 case WM_SYSCOMMAND:
  {
   switch(wParam)
   {
   case SC_MINIMIZE:
    {
     Minimize(hDlg);
     return TRUE;
    } break;
   case SC_RESTORE:
    { return FALSE; } break;
   case SC_MAXIMIZE:
    { return FALSE; } break;
   case SC_CLOSE:
     return OnClose(hDlg);
   }
  } break;
 case WM_COMMAND:
  {
   switch(HIWORD(wParam)) // message source
   {
   case 0: // menu
    {
     switch(LOWORD(wParam)) // menu identifier
     {
					case ID_MENU_ACTIVATE:
						{
							Activate(hDlg);
							isProcessed = TRUE;
						} break;
					case ID_MENU_EXIT:
							return OnClose(hDlg);
     }
    } break;
   case 1: // accelerator
				{
					/*switch(LOWORD(wParam)) // accelerator identifier
     {}*/
				} break;
   default: // control
    {
     /*switch(LOWORD(wParam)) // control identifier
     {
     default:
      {
							//HWND hControl = reinterpret_cast<HWND>(lParam); // handle to the control window
							return FALSE;
						}
					}*/
    }
   }
		} break;
 case WM_USER:
  {
   switch(LOWORD(lParam))
   {
   case WM_LBUTTONDOWN:
    {
     Activate(hDlg);
     isProcessed = TRUE;
    } break;
   case WM_RBUTTONUP:
    {
     POINT CursorPosition;
     GetCursorPos(&CursorPosition);

     HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDM_POPUP));
     HMENU hPopupMenu = GetSubMenu(hMenu, 0);

					EnableMenuItem(hPopupMenu, ID_MENU_ACTIVATE, MF_BYCOMMAND | ( GetWindowState()->IsVisible() ? MF_GRAYED : MF_ENABLED ));

					SetForegroundWindow(hDlg);
					TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, CursorPosition.x, CursorPosition.y, NULL, hDlg, NULL);
					PostMessage(hDlg, WM_NULL, 0, 0); // see MSDN TrackPopupMenu Function Remarks

					isProcessed = TRUE;
    } break;
   }
  } break;
 case WM_CLOSE:
  return OnClose(hDlg);
 }
 // ќстальные сообщени€ обрабатываютс€ операционной системой (процедурой диалогового окна по-умолчанию).
 return isProcessed;
}