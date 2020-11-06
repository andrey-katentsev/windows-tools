#pragma once

#include <windows.h>

typedef struct tagFileCopyData
{
 // Данные для копирования файлов.
 TCHAR *lpSourceFileName;
 TCHAR *lpDestinationFileName;
 HANDLE hSourceFile;
 HANDLE hDestinationFile;
 // Данные для управления элементами управления.
 HWND hProgressControl;
 HWND hCopyButtonControl;
 HWND hCancelButtonControl;
 HWND hSourceButtonControl;
 HWND hDestinationButtonControl;
 HWND hStatusTextControl;
} HS_FILECOPYDATA, *HS_LPFILECOPYDATA;

// Процедура потока для копирования файла.
DWORD WINAPI hsFileCopyRoutine(LPVOID lpParameter);