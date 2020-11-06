#pragma once

#include <windows.h>

typedef struct tagFileCopyData
{
 // ������ ��� ����������� ������.
 TCHAR *lpSourceFileName;
 TCHAR *lpDestinationFileName;
 HANDLE hSourceFile;
 HANDLE hDestinationFile;
 // ������ ��� ���������� ���������� ����������.
 HWND hProgressControl;
 HWND hCopyButtonControl;
 HWND hCancelButtonControl;
 HWND hSourceButtonControl;
 HWND hDestinationButtonControl;
 HWND hStatusTextControl;
} HS_FILECOPYDATA, *HS_LPFILECOPYDATA;

// ��������� ������ ��� ����������� �����.
DWORD WINAPI hsFileCopyRoutine(LPVOID lpParameter);