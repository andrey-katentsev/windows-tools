#include <windows.h>
#include <commctrl.h>

#include "hsFileCopyRoutine.h"

#define FCR_MIN_RANGE 0
#define FCR_RANGE_STEP 2048

#define FCR_STEP_INCREMENT 1

#define FCR_READY_TEXT TEXT("Подготовка к копированию...")
#define FCR_IN_PROGRESS_TEXT TEXT("Идёт копирование...")
#define FCR_DONE_TEXT TEXT("Копирование завершено.")

#define FCR_UNABLE_TO_OPEN_SOURCE_FILE TEXT("Ошибка доступа к исходному файлу.")
#define FCR_UNABLE_TO_OPEN_DESTINATION_FILE TEXT("Ошибка доступа к копии файа.")

DWORD WINAPI hsFileCopyRoutine(LPVOID lpParameter)
{
 //UNREFERENCED_PARAMETER(lpParameter);
 HS_LPFILECOPYDATA lpData=(HS_LPFILECOPYDATA)lpParameter;

 // Количество прочитанных/записанных байт.
 DWORD dwBytesRead=0;
 DWORD dwBytesWritten=0;

 // Буффер чтения/записи.
 BYTE bData[FCR_RANGE_STEP];
 SecureZeroMemory(bData,sizeof(bData));

 // Настраиваем элементы управления.
 EnableWindow((*lpData).hCopyButtonControl,FALSE);
 EnableWindow((*lpData).hCancelButtonControl,TRUE);
 EnableWindow((*lpData).hSourceButtonControl,FALSE);
 EnableWindow((*lpData).hDestinationButtonControl,FALSE);

 SendMessage((*lpData).hStatusTextControl,WM_SETTEXT,NULL,(LPARAM)FCR_READY_TEXT);

 // Копируем файл.
 HANDLE hSFile=CreateFile((*lpData).lpSourceFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
 if(INVALID_HANDLE_VALUE==hSFile)
 {
  SendMessage((*lpData).hStatusTextControl,WM_SETTEXT,NULL,(LPARAM)FCR_UNABLE_TO_OPEN_SOURCE_FILE);
  return 0;
 }

 HANDLE hDFile=CreateFile((*lpData).lpDestinationFileName,GENERIC_WRITE,NULL,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
 if(INVALID_HANDLE_VALUE==hDFile)
 {
  SendMessage((*lpData).hStatusTextControl,WM_SETTEXT,NULL,(LPARAM)FCR_UNABLE_TO_OPEN_DESTINATION_FILE);
  return 0;
 }

 (*lpData).hSourceFile=hSFile;
 (*lpData).hDestinationFile=hDFile;

 // Получаем размер исходного файла.
 DWORD dwFileSize=GetFileSize(hSFile,NULL);
 
 // Устанавливаем диапазон и шаг для полоски состояния.
 // Set the range and increment of the progress bar.

 //WORD wMinRange=FCR_MIN_RANGE;
 //WORD wMaxRange=(WORD)(dwFileSize/FCR_RANGE_STEP);

 //SendMessage((*lpData).hProgressControl,PBM_SETRANGE,NULL,MAKELPARAM(wMinRange,wMaxRange));

 DWORD dwMinRange=FCR_MIN_RANGE;
 DWORD dwMaxRange=dwFileSize/FCR_RANGE_STEP;

 SendMessage((*lpData).hProgressControl,PBM_SETRANGE32,dwMinRange,dwMaxRange);
 SendMessage((*lpData).hProgressControl,PBM_SETSTEP,FCR_STEP_INCREMENT,NULL);

 SendMessage((*lpData).hStatusTextControl,WM_SETTEXT,NULL,(LPARAM)FCR_IN_PROGRESS_TEXT);

 do
 {
  // Читаем данные и записываем их в файл.
  ReadFile(hSFile,bData,FCR_RANGE_STEP,&dwBytesRead,NULL);
  WriteFile(hDFile,bData,dwBytesRead,&dwBytesWritten,NULL);

  // Изменяем состояние полоски.
  // Advance the current position of the progress bar by the increment.
  SendMessage((*lpData).hProgressControl,PBM_STEPIT,NULL,NULL);
 }
 while(dwBytesRead);

 CloseHandle(hSFile);
 CloseHandle(hDFile);

 // Настраиваем элементы управления.
 EnableWindow((*lpData).hCopyButtonControl,TRUE);
 EnableWindow((*lpData).hCancelButtonControl,FALSE);
 EnableWindow((*lpData).hSourceButtonControl,TRUE);
 EnableWindow((*lpData).hDestinationButtonControl,TRUE);

 SendMessage((*lpData).hStatusTextControl,WM_SETTEXT,NULL,(LPARAM)FCR_DONE_TEXT);

 return 0;
}

