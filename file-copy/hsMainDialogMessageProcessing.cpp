#include <windows.h>
#include <commctrl.h>

#include <tchar.h>

#include "Resource.h"

#include "hsFileCopyRoutine.h"
#include "hsThreadData.h"

#pragma region MACRO Definitions
#define MD_NO_PATH_SELECTED 0

#define MD_SAME_PATH 0

#define MD_SAME_PATH_TEXT TEXT("���� ���������� �� ������ ��������� � ������ ��������� �����. ����������, �������� ������ ����.")
#define MD_SAME_PATH_CAPTION TEXT("����������� �����")

#define MD_BALLOONTIP_TITLE TEXT("����������� �����")
#define MD_BALLOONTIP_TEXT TEXT("����������, �������� ���� � �����.")

#define MD_OPEN_DIALOGBOX_TITLE TEXT("�������� �������� ����")
#define MD_SAVE_DIALOGBOX_TITLE TEXT("�������� ���� ����������")

#define MD_MAX_TITLE 256
#define MD_MAX_PATH 1024

#define MD_FILTER_INDEX 1
#define MD_FILENAME_FILTER TEXT("��� �����\0*.*\0\0")

#define MD_CANCELLED_TEXT TEXT("����������� �������� �������������.")

#define MD_CLEAR_BOUNDS 0
#pragma endregion

#pragma region GLOBAL DATA
TCHAR tcSPath[MD_MAX_PATH];
TCHAR tcDPath[MD_MAX_PATH];
#pragma endregion

#pragma region CONTROL
// LOWORD(wParam) - ������������� �������� ���������� (Control identifier).
// HIWORD(wParam) - ��� ����������� (Control-defined notification code).
//        lParam  - ��������� �� ���� �������� ���������� (Handle to the control window).
BOOL MDProcessControlMessage(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
 UNREFERENCED_PARAMETER(hDlg);
 //UNREFERENCED_PARAMETER(wParam);
 UNREFERENCED_PARAMETER(lParam);

 // ������.
 OPENFILENAME stFileName;
 EDITBALLOONTIP stBalloonTip;

 TCHAR tcTitle[MD_MAX_TITLE];

 HINSTANCE hInstance=(HINSTANCE)GetWindowLong(hDlg,GWL_HINSTANCE);

 BOOL isResult=FALSE;

 SecureZeroMemory(&stFileName,sizeof(stFileName));
 SecureZeroMemory(&stBalloonTip,sizeof(stBalloonTip));

 SecureZeroMemory(tcSPath,sizeof(tcSPath));
 SecureZeroMemory(tcDPath,sizeof(tcDPath));
 SecureZeroMemory(tcTitle,sizeof(tcTitle));

 switch(LOWORD(wParam))
 {
 case IDC_SELECT_SOURCE_BUTTON:
  {
   // �������������� �������� �����.
   stFileName.lStructSize=sizeof(OPENFILENAME);
   stFileName.hwndOwner=hDlg;
   stFileName.hInstance=hInstance;
   stFileName.lpstrFilter=MD_FILENAME_FILTER;
   stFileName.nFilterIndex=MD_FILTER_INDEX;
   stFileName.lpstrFile=tcSPath;
   stFileName.nMaxFile=MD_MAX_PATH;
   stFileName.lpstrFileTitle=tcTitle;
   stFileName.nMaxFileTitle=MD_MAX_TITLE;
   stFileName.lpstrTitle=MD_OPEN_DIALOGBOX_TITLE;
   stFileName.Flags=
    OFN_PATHMUSTEXIST|
    OFN_FILEMUSTEXIST|
    OFN_DONTADDTORECENT|
    OFN_NONETWORKBUTTON;
    //OFN_READONLY
    //OFN_HIDEREADONLY;

   // ��������� ������ ��� ��������� ����� �����.
   isResult=GetOpenFileName(&stFileName);
   // ��������� ���� ����� ����������� �������.
   if(isResult)
   { SetDlgItemText(hDlg,IDC_SOURCE_EDIT,stFileName.lpstrFile); }
   return TRUE;
  } break;
 case IDC_SELECT_DESTINATION_BUTTON:
  {
   // �������������� �������� �����.
   stFileName.lStructSize=sizeof(OPENFILENAME);
   stFileName.hwndOwner=hDlg;
   stFileName.hInstance=hInstance;
   stFileName.lpstrFilter=MD_FILENAME_FILTER;
   stFileName.nFilterIndex=MD_FILTER_INDEX;
   stFileName.lpstrFile=tcDPath;
   stFileName.nMaxFile=MD_MAX_PATH;
   stFileName.lpstrFileTitle=tcTitle;
   stFileName.nMaxFileTitle=MD_MAX_TITLE;
   stFileName.lpstrTitle=MD_SAVE_DIALOGBOX_TITLE;
   stFileName.Flags=
    OFN_PATHMUSTEXIST|
    OFN_CREATEPROMPT|
    OFN_NOREADONLYRETURN|
    OFN_OVERWRITEPROMPT|
    OFN_DONTADDTORECENT|
    OFN_NONETWORKBUTTON|
    OFN_HIDEREADONLY;

   // ��������� ������ ��� ��������� ����� �����.
   isResult=GetSaveFileName(&stFileName);
   // ��������� ���� ����� ����������� �������.
   if(isResult)
   { SetDlgItemText(hDlg,IDC_DESTINATION_EDIT,stFileName.lpstrFile); }
   return TRUE;
  } break;
 case IDC_COPY_BUTTON:
  {
   stBalloonTip.cbStruct=sizeof(EDITBALLOONTIP);
   stBalloonTip.pszTitle=MD_BALLOONTIP_TITLE;
   stBalloonTip.ttiIcon=TTI_INFO;
   stBalloonTip.pszText=MD_BALLOONTIP_TEXT;

   // ���� �� ������ ���� � ��������� �����.
   if(MD_NO_PATH_SELECTED==GetDlgItemText(hDlg,IDC_SOURCE_EDIT,tcSPath,MD_MAX_PATH))
   {
    HWND hControl=GetDlgItem(hDlg,IDC_SOURCE_EDIT);
    Edit_ShowBalloonTip(hControl,&stBalloonTip);
    //hControl=GetDlgItem(hDlg,IDC_SELECT_SOURCE_BUTTON);
    //SetFocus(hControl);
    return TRUE;
   }

   // ���� �� ������ ���� � �������� �����.
   if(MD_NO_PATH_SELECTED==GetDlgItemText(hDlg,IDC_DESTINATION_EDIT,tcDPath,MD_MAX_PATH))
   {
    HWND hControl=GetDlgItem(hDlg,IDC_DESTINATION_EDIT);
    Edit_ShowBalloonTip(hControl,&stBalloonTip);
    //hControl=GetDlgItem(hDlg,IDC_SELECT_DESTINATION_BUTTON);
    //SetFocus(hControl);
    return TRUE;
   }

   // �������� ���� �������� ������ ����������.
   if(MD_SAME_PATH==_tcsicmp(tcSPath,tcDPath))
   {
    MessageBox(hDlg,MD_SAME_PATH_TEXT,MD_SAME_PATH_CAPTION,MB_ICONWARNING|MB_OK);
    return TRUE;
   }

   // �������������� ������ ������.
   stThreadData.lpSourceFileName=tcSPath;
   stThreadData.lpDestinationFileName=tcDPath;
   stThreadData.hProgressControl=GetDlgItem(hDlg,IDC_STATUS_PROGRESS);
   stThreadData.hCopyButtonControl=GetDlgItem(hDlg,IDC_COPY_BUTTON);
   stThreadData.hCancelButtonControl=GetDlgItem(hDlg,IDC_CANCEL_BUTTON);
   stThreadData.hSourceButtonControl=GetDlgItem(hDlg,IDC_SELECT_SOURCE_BUTTON);
   stThreadData.hDestinationButtonControl=GetDlgItem(hDlg,IDC_SELECT_DESTINATION_BUTTON);
   stThreadData.hStatusTextControl=GetDlgItem(hDlg,IDC_STATUS_TEXT_STATIC);

   // ������ ����� ��� ����������� �����.
   hThread=CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)hsFileCopyRoutine,(LPVOID)&stThreadData,NULL,&dwThreadID);

   return TRUE;
  } break;
 case IDC_CANCEL_BUTTON:
  {
   // ��������� �����.
   TerminateThread(hThread,FALSE);
			CloseHandle(hThread);
   CloseHandle(stThreadData.hSourceFile);
   CloseHandle(stThreadData.hDestinationFile);

   // ����������� �������� ����������.
   EnableWindow(GetDlgItem(hDlg,IDC_COPY_BUTTON),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_CANCEL_BUTTON),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_SELECT_SOURCE_BUTTON),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_SELECT_DESTINATION_BUTTON),TRUE);

   SendMessage(GetDlgItem(hDlg,IDC_STATUS_TEXT_STATIC),WM_SETTEXT,NULL,(LPARAM)MD_CANCELLED_TEXT);
   SendMessage(GetDlgItem(hDlg,IDC_STATUS_PROGRESS),PBM_SETPOS,MD_CLEAR_BOUNDS,NULL);
   return TRUE;
  } break;
 }
 return FALSE;
}
#pragma endregion

#pragma region ACCELERATOR
// LOWORD(wParam) - ������������� ������������ (Accelerator identifier).
// HIWORD(wParam) - 1::const;
//        lParam  - 0::const;
BOOL MDProcessAcceleratorMessage(HWND hDlg,WPARAM wParam)
{
 UNREFERENCED_PARAMETER(hDlg);
 UNREFERENCED_PARAMETER(wParam);
 return FALSE;
}
#pragma endregion

#pragma region MENU
// LOWORD(wParam) - ������������� �������� ���� (Menu identifier).
// HIWORD(wParam) - 0::const;
//        lParam  - 0::const;
BOOL MDProcessMenuMessage(HWND hDlg,WPARAM wParam)
{
 UNREFERENCED_PARAMETER(hDlg);
 UNREFERENCED_PARAMETER(wParam);
 return FALSE;
}
#pragma endregion

#pragma region HOTKEYS
//        wParam  - (Specifies the identifier of the hot key that generated the message).
// LOWORD(lParam) - (The low-order word specifies the keys that were to be pressed in combination with the key specified by the high-order word to generate the WM_HOTKEY message).
// HIWORD(lParam) - (The high-order word specifies the virtual key code of the hot key).
BOOL MDProcessHotKeyMessage(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
 UNREFERENCED_PARAMETER(hDlg);

 switch(wParam)
 {
 case IDHOT_SNAPDESKTOP:
 case IDHOT_SNAPWINDOW:
  {
   // ...
  } break;
 }

 switch(LOWORD(lParam))
 {
 case MOD_ALT:
 case MOD_CONTROL:
 case MOD_SHIFT:
 case MOD_WIN:
  {
   // ...
  } break;
 }
 return FALSE;
}
#pragma endregion