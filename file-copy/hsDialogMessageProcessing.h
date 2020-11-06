#pragma once

#include <windows.h>

// Функции обработчики сообщений.
// Message-Handling / Message-Processing Functions.

// Любая из этих функций должна возвращать TRUE, если она обработала сообщение и FALSE в противном случае.
// These functions should return TRUE, if it processes message and FALSE otherwise.

// ГЛАВНЫЙ Диалог.
// MAIN Dialog.
BOOL MDProcessMenuMessage(HWND hDlg,WPARAM wParam);
BOOL MDProcessAcceleratorMessage(HWND hDlg,WPARAM wParam);
BOOL MDProcessControlMessage(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL MDProcessHotKeyMessage(HWND hDlg,WPARAM wParam,LPARAM lParam);