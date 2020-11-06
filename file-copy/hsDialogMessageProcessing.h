#pragma once

#include <windows.h>

// ������� ����������� ���������.
// Message-Handling / Message-Processing Functions.

// ����� �� ���� ������� ������ ���������� TRUE, ���� ��� ���������� ��������� � FALSE � ��������� ������.
// These functions should return TRUE, if it processes message and FALSE otherwise.

// ������� ������.
// MAIN Dialog.
BOOL MDProcessMenuMessage(HWND hDlg,WPARAM wParam);
BOOL MDProcessAcceleratorMessage(HWND hDlg,WPARAM wParam);
BOOL MDProcessControlMessage(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL MDProcessHotKeyMessage(HWND hDlg,WPARAM wParam,LPARAM lParam);