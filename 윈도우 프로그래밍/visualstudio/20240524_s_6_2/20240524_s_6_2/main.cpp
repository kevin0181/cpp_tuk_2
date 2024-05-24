#include <Windows.h>
#include <tchar.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Calculator";
LPCTSTR lpszWindowName = L"Calculator";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

TCHAR input[256] = L"";
TCHAR operation = 0;
double result = 0.0;
BOOL new_input = TRUE;

void AddToInput(HWND hDlg, TCHAR ch) {
    if (new_input) {
        _tcscpy_s(input, L"");
        new_input = FALSE;
    }
    int len = _tcslen(input);
    if (len < 255) {
        input[len] = ch;
        input[len + 1] = 0;
        SetDlgItemText(hDlg, IDC_EDIT1, input);
    }
}

void ExecuteOperation(HWND hDlg) {
    double num = _tstof(input);
    switch (operation) {
    case '+': result += num; break;
    case '-': result -= num; break;
    case '*': result *= num; break;
    case '/': if (num != 0) result /= num; break;
    default: result = num; break;
    }
    _stprintf_s(input, L"%f", result);
    SetDlgItemText(hDlg, IDC_EDIT1, input);
    new_input = TRUE;
}

BOOL CALLBACK DialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    switch (iMsg) {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT1, input);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BUTTON1: AddToInput(hDlg, '0'); break;
        case IDC_BUTTON2: AddToInput(hDlg, '1'); break;
        case IDC_BUTTON3: AddToInput(hDlg, '2'); break;
        case IDC_BUTTON4: operation = '/'; ExecuteOperation(hDlg); break;
        case IDC_BUTTON5: AddToInput(hDlg, '3'); break;
        case IDC_BUTTON6: AddToInput(hDlg, '4'); break;
        case IDC_BUTTON7: AddToInput(hDlg, '5'); break;
        case IDC_BUTTON8: operation = '*'; ExecuteOperation(hDlg); break;
        case IDC_BUTTON9: AddToInput(hDlg, '6'); break;
        case IDC_BUTTON10: AddToInput(hDlg, '7'); break;
        case IDC_BUTTON11: AddToInput(hDlg, '8'); break;
        case IDC_BUTTON12: operation = '-'; ExecuteOperation(hDlg); break;
        case IDC_BUTTON13: AddToInput(hDlg, '9'); break;
        case IDC_BUTTON14: AddToInput(hDlg, '.'); break;
        case IDC_BUTTON15: operation = '+'; ExecuteOperation(hDlg); break;
        case IDC_BUTTON17: SetDlgItemText(hDlg, IDC_EDIT1, L""); break; // CE
        case IDC_BUTTON18: result = 0; operation = 0; SetDlgItemText(hDlg, IDC_EDIT1, L""); break; // C
        case IDC_BUTTON19: ExecuteOperation(hDlg); operation = 0; break; // =
        case IDC_BUTTON20: // Backspace
            if (_tcslen(input) > 0) {
                input[_tcslen(input) - 1] = 0;
                SetDlgItemText(hDlg, IDC_EDIT1, input);
            }
            break;
        case IDC_BUTTON24: PostQuitMessage(0); break; // Finish
        }
        return TRUE;
    case WM_CLOSE:
        PostQuitMessage(0);
        return TRUE;
    }
    return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
    MSG Message;
    g_hInst = hInstance;

    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DialogProc);

    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return static_cast<int>(Message.wParam);
}
