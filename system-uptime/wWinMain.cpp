#include <windows.h>
#include <commctrl.h>
#include "Resource.h"
// comctl32.lib

LRESULT CALLBACK SystemUpTime(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int)
{
 INITCOMMONCONTROLSEX ICC;

 // Регистрируем дополнительные классы элементов управления.
 ICC.dwSize=sizeof(ICC);
 ICC.dwICC=ICC_WIN95_CLASSES;

 if(!InitCommonControlsEx(&ICC))
 { ::MessageBox(NULL, TEXT("InitCommonControlsEx()=FALSE"), TEXT("Инициализация"), MB_OK); }

 return DialogBoxW(hInstance, MAKEINTRESOURCE(IDD_SYSTEM_UPTIME), NULL, (DLGPROC)SystemUpTime);
}