#include <windows.h>
#include "contact.h"

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("Address Book");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass{};
    
    wndclass.style = CS_HREDRAW, CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE (IDI_ICON1));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 20);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName, TEXT("Address Book"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        355, 500, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        CreateInterface(hwnd);
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON_SAVE)
        {
            SaveContact(hwnd);
        }
        else if (LOWORD(wParam) == ID_BUTTON_LOAD)
        {
            LoadContact(hwnd);
        }
        else if (LOWORD(wParam) == ID_BUTTON_SEARCH)
        {
            SearchContact();
        }
        else if (LOWORD(wParam) == ID_BUTTON_DELETE)
        {
            int selectedIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
            DeleteContact(hwnd, selectedIndex);
        }
        else if (LOWORD(wParam) == ID_BUTTON_EDIT) {
            // Получение выбранного индекса информации в ListBox
            int selectedIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);

            if (selectedIndex != LB_ERR)
            {
                // Получение текста выбранной строки
                int textLength = SendMessage(hListBox, LB_GETTEXTLEN, selectedIndex, 0);
                std::vector<wchar_t> buffer(textLength + 1);
                SendMessage(hListBox, LB_GETTEXT, selectedIndex, reinterpret_cast<LPARAM>(buffer.data()));
                std::wstring selectedText = buffer.data();

                // Проверка на недопустимую строку
                if (selectedText == L"----------------------------------------") {
                    MessageBox(hwnd, L"You cannot edit this line.", L"Error", MB_ICONERROR);
                }
                else {
                    // Открытие диалогового окна редактирования
                    OpenEditDialog(hwnd, selectedIndex);
                }
            }
            // Проверка на выбор контакта
            if (selectedIndex == LB_ERR)
            {
                MessageBox(hwnd, L"Please select a contact to edit.", L"Error", MB_ICONERROR);
            }
        }
        else if (LOWORD(wParam) == ID_BUTTON_SORT)
        {
            SortContactsByLastName(hwnd);
        }
         break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
