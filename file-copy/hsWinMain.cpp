#pragma message("Last modified on " __TIMESTAMP__ )

#define ISOLATION_AWARE_ENABLED 1
#define MANIFEST_RESOURCE_ID 2

#include <windows.h>
#include <commctrl.h>

#include "Resource.h"
#include "hsDialogProcedure.h"

// If the function succeeds, terminating when it receives a WM_QUIT message, it should return the exit value contained in that message's wParam parameter.
// If the function terminates before entering the message loop, it should return zero.
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPreviousInstance,LPSTR lpCmdLine,int nCmdShow)
{
 UNREFERENCED_PARAMETER(hPreviousInstance);
 UNREFERENCED_PARAMETER(lpCmdLine);
 UNREFERENCED_PARAMETER(nCmdShow);

 INITCOMMONCONTROLSEX ICC;

 // Регистрируем дополнительные классы элементов управления.
 ICC.dwSize=sizeof(INITCOMMONCONTROLSEX);
 ICC.dwICC=ICC_WIN95_CLASSES;

 if(!InitCommonControlsEx(&ICC))
 {
  MessageBox(NULL,TEXT("InitCommonControlsEx()=FALSE"),TEXT("Инициализация"),MB_ICONERROR|MB_OK);
  //return 0;
 }

 // Создаём модальное диалоговое окно.
 DialogBox(hInstance,MAKEINTRESOURCE(IDD_MAIN),NULL,hsMainDialogProcedure);
 return 0;
}