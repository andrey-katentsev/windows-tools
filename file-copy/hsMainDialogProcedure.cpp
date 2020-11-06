#include <windows.h>
#include <commctrl.h>
#include "Resource.h"
//#include "hsDefinitions.h"
#include "hsDialogMessageProcessing.h"

#define MD_DLG_CLOSE 1

// ��� �������, ��������� ����������� ���� ���������� �������� TRUE, ���� ��� ���������� ���������, � FALSE � ��������� ������.
// Typically, the dialog box procedure should return TRUE if it processed the message, and FALSE if it did not.
// If the dialog box procedure returns FALSE, the dialog manager performs the default dialog operation in response to the message.
// If the dialog box procedure processes a message that requires a specific return value, the dialog box procedure should set the desired return value by calling SetWindowLong(hWndDlg,DWL_MSGRESULT,lResult) immediately before returning TRUE.
// Note that you must call SetWindowLong immediately before returning TRUE; doing so earlier may result in the DWL_MSGRESULT value being overwritten by a nested dialog box message.
INT_PTR CALLBACK hsMainDialogProcedure(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
 HINSTANCE hInstance=(HINSTANCE)GetWindowLong(hDlg,GWL_HINSTANCE);

 switch(uMsg)
 {
 case WM_INITDIALOG:
 // ��������� ����������� ���� ������ ������� TRUE, ����� ������� ������� ���������� ������������ ����� �� ������� ���������� ��������� � wParam.
 // ����� ��� ������ ������� FALSE, ����� ������������� ��������� ������������� ������ ��-���������. (���������� ����� ���������� FALSE ������, ���� ��� ���������� ������������ ����� �� ���� �� ��������� ���������� ����������� ����).
 // The dialog box procedure should return TRUE to direct the system to set the keyboard focus to the control specified by wParam.
 // Otherwise, it should return FALSE to prevent the system from setting the default keyboard focus. (An application can return FALSE only if it has set the keyboard focus to one of the controls of the dialog box).
  {
   // �������� �����������.
   HANDLE hIcon=LoadImage(hInstance,MAKEINTRESOURCE(IDI_MAIN),IMAGE_ICON,0,0,LR_DEFAULTSIZE);
   // ��������� ������ ����������� ����.
   SendMessage(hDlg,WM_SETICON,ICON_BIG,(LPARAM)hIcon);
   SendMessage(hDlg,WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
   // ������������� ����� �����
   SetFocus(hDlg);

   // ����������� ������
   HWND hButton;
   hButton=GetDlgItem(hDlg,IDC_CANCEL_BUTTON);
   EnableWindow(hButton,FALSE);

   // The dialog box procedure should return the value directly.
   return FALSE;
  } break;
 case WM_COMMAND:
 // ���� ���������� ���������� ��� ���������, �� ��� ������ ������� 0.
 // If an application processes this message, it should return zero.
  {
   // ���������� �������� ���������.
   switch(lParam)
   {
   case 0:
    {
     switch(HIWORD(wParam))
     {
     case 0: // ������� ����.
      {
       if(MDProcessMenuMessage(hDlg,wParam))
       { SetWindowLong(hDlg,DWL_MSGRESULT,NULL); return TRUE; }
      } break;
     case 1: // ������� ������������.
      {
       if(MDProcessAcceleratorMessage(hDlg,wParam))
       { SetWindowLong(hDlg,DWL_MSGRESULT,NULL); return TRUE; }
      } break;
     }
    } break;
   default: // ������� �������� ����������.
    {
     if(MDProcessControlMessage(hDlg,wParam,lParam))
     { SetWindowLong(hDlg,DWL_MSGRESULT,NULL); return TRUE; }
    } break;
   }
  } break;
 case WM_HOTKEY: // ������ �������� ������.
  {
   // ��� ������������� ��������.
   // No return value.
   MDProcessHotKeyMessage(hDlg,wParam,lParam);
   return TRUE;
  } break;
 case WM_CLOSE:
  // ���� ���������� ���������� ��� ���������, �� ��� ������ ������� 0.
  // If an application processes this message, it should return zero.
  {
   EndDialog(hDlg,MD_DLG_CLOSE);
   SetWindowLong(hDlg,DWL_MSGRESULT,NULL);
   return TRUE;
  } break;
 }
 // ��������� ��������� ���������� �� ��������� ��������� ����������� ���� ��-���������.
 return FALSE;
}