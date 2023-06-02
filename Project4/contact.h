#include <Windows.h>
#include <fstream>
#include <string>
#include "resource.h"
#include <vector>
#include <algorithm> 
#include <locale>

#define ID_EDIT_NAME 100
#define ID_EDIT_HOME_ADDRESS 101
#define ID_EDIT_WORK_ADDRESS 102
#define ID_EDIT_PHONE_NUMBER 103
#define ID_EDIT_ADDITIONAL_INFO 104
#define ID_BUTTON_SAVE 105
#define ID_BUTTON_LOAD 107
#define ID_LISTBOX 108
#define ID_BUTTON_SEARCH 109
#define ID_EDIT_SEARCH 110
#define ID_BUTTON_DELETE 111
#define ID_BUTTON_EDIT 112
#define ID_BUTTON_SORT 113
//���� ��� ������ ���������
static HWND hEditName, hEditHomeAddress, hEditWorkAddress, hEditPhoneNumber, hEditAdditionalInfo;
static HWND hButtonSave, hButtonLoad, hListBox, hEditSearch, hButtonSearch,hButtonDelete,hButtonEdit,hButtonSort;
void CreateInterface(HWND hwnd); // �������� ������� ��������� ���������
void LoadContact(HWND hwnd);//�������� ������� ������������ ���� ����� � hListBox
void SearchContact();//�������� ������� ������ ��������
void DeleteContact(HWND hwnd, int selectedIndex);//�������� ������� ��������� ��������
void SaveEditedContact(const std::wstring& originalText, const std::wstring& editedText);//�������� ������� ���������� ��������
INT_PTR CALLBACK EditDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//��������� ���� ��� ����������� 
void OpenEditDialog(HWND hwnd, int selectedIndex);//�������� ������� ��� ������� ���������� ����
bool UkrainianLastNameComparator(const std::wstring& lastName1, const std::wstring& lastName2);//�������� ������� ��� ���������� ���������� ��������
void SortContactsByLastName(HWND hwnd);//�������� ������� ��� ���������� �� ��������
void CreateInterface(HWND hwnd) {

    CreateWindow(TEXT("static"), TEXT("Full Name:"),
        WS_CHILD | WS_VISIBLE,
        20, 20, 100, 20, hwnd, NULL, NULL, NULL);
    hEditName = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        120, 20, 200, 20, hwnd, (HMENU)ID_EDIT_NAME, NULL, NULL);

    CreateWindow(TEXT("static"), TEXT("Home Address:"),
        WS_CHILD | WS_VISIBLE,
        20, 50, 100, 20, hwnd, NULL, NULL, NULL);
    hEditHomeAddress = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        120, 50, 200, 20, hwnd, (HMENU)ID_EDIT_HOME_ADDRESS, NULL, NULL);

    CreateWindow(TEXT("static"), TEXT("Work Address:"),
        WS_CHILD | WS_VISIBLE,
        20, 80, 100, 20, hwnd, NULL, NULL, NULL);
    hEditWorkAddress = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        120, 80, 200, 20, hwnd, (HMENU)ID_EDIT_WORK_ADDRESS, NULL, NULL);

    CreateWindow(TEXT("static"), TEXT("Phone Number:"),
        WS_CHILD | WS_VISIBLE,
        20, 110, 100, 20, hwnd, NULL, NULL, NULL);
    hEditPhoneNumber = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        120, 110, 200, 20, hwnd, (HMENU)ID_EDIT_PHONE_NUMBER, NULL, NULL);

    CreateWindow(TEXT("static"), TEXT("Additional Info:"),
        WS_CHILD | WS_VISIBLE,
        20, 140, 100, 20, hwnd, NULL, NULL, NULL);
    hEditAdditionalInfo = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        120, 140, 200, 20, hwnd, (HMENU)ID_EDIT_ADDITIONAL_INFO, NULL, NULL);

    hButtonSave = CreateWindow(TEXT("button"), TEXT("Save"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        20, 170, 50, 20, hwnd, (HMENU)ID_BUTTON_SAVE, NULL, NULL);
    hButtonLoad = CreateWindow(TEXT("button"), TEXT("Load"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        80, 170, 50, 20, hwnd, (HMENU)ID_BUTTON_LOAD, NULL, NULL);
    hListBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"ListBox", NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL,
        20, 240, 300, 200, hwnd, NULL, NULL, NULL);
    hEditSearch = CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
        20, 200, 240, 25, hwnd, (HMENU)ID_EDIT_SEARCH, GetModuleHandle(NULL), NULL);
    hButtonSearch = CreateWindow(TEXT("button"), TEXT("Search"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        270, 200, 50, 25, hwnd, (HMENU)ID_BUTTON_SEARCH, NULL, NULL);
    hButtonDelete = CreateWindow(TEXT("button"), TEXT("Delete"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        140, 170, 50, 20, hwnd, (HMENU)ID_BUTTON_DELETE, NULL, NULL);
    hButtonEdit = CreateWindow(TEXT("button"), TEXT("Edit"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        200, 170, 50, 20, hwnd, (HMENU)ID_BUTTON_EDIT, NULL, NULL);
    hButtonSort = CreateWindow(TEXT("button"), TEXT("Sort"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        260, 170, 50, 20, hwnd, (HMENU)ID_BUTTON_SORT, NULL, NULL);
}
void SaveContact(HWND hwnd) {
    std::locale::global(std::locale("uk_UA.UTF-8"));
    
    std::wstring fullname, homeAddress, workAddress, phoneNumber, additionalInfo;

    // �������� ����� �� ���� ����� � ��������� ��� � std::wstring
    const int bufferSize = 100;
    wchar_t buffer[bufferSize];
    GetWindowTextW(hEditName, buffer, bufferSize);
    fullname = buffer;
    GetWindowTextW(hEditHomeAddress, buffer, bufferSize);
    homeAddress = buffer;
    GetWindowTextW(hEditWorkAddress, buffer, bufferSize);
    workAddress = buffer;
    GetWindowTextW(hEditPhoneNumber, buffer, bufferSize);
    phoneNumber = buffer;
    GetWindowTextW(hEditAdditionalInfo, buffer, bufferSize);
    additionalInfo = buffer;

    if (fullname.empty() || homeAddress.empty() || workAddress.empty() || phoneNumber.empty() || additionalInfo.empty()) {
        // ������� ��������� �� ������, ���� ���� �� ����� �� ��������
        MessageBoxW(hwnd, L"Please fill out all fields", L"Error", MB_ICONERROR);
    }
    else {
        // ��������� ���� ��� ������ � ��������� �� ���������
        std::wifstream infile("address_book.txt");
        std::wstring line;
        bool duplicateFound = false;

        while (std::getline(infile, line)) {
            if (line.find(fullname) != std::wstring::npos &&
                line.find(homeAddress) != std::wstring::npos &&
                line.find(workAddress) != std::wstring::npos &&
                line.find(phoneNumber) != std::wstring::npos &&
                line.find(additionalInfo) != std::wstring::npos) {
                duplicateFound = true;
                break;
            }
        }

        infile.close();

        if (duplicateFound) {
            // ������� ��������� �� ������, ���� ������ ��������
            MessageBoxW(hwnd, L"This entry already exists in the address book", L"Error", MB_OK);
        }
        else
        {
            std::wofstream outfile;
            outfile.open("address_book.txt", std::ios::app);
            if (outfile.is_open())
            {
                outfile << L"Full Name: " << fullname << std::endl;
                outfile << L"Home Address: " << homeAddress << std::endl;
                outfile << L"Work Address: " << workAddress << std::endl;
                outfile << L"Phone Number: " << phoneNumber << std::endl;
                outfile << L"Additional Info: " << additionalInfo << std::endl;
                outfile << L"----------------------------------------" << std::endl;
                outfile.close();
                SetWindowTextW(hEditName, L"");
                SetWindowTextW(hEditHomeAddress, L"");
                SetWindowTextW(hEditWorkAddress, L"");
                SetWindowTextW(hEditPhoneNumber, L"");
                SetWindowTextW(hEditAdditionalInfo, L"");
                LoadContact(hwnd);
            }
            else
            {
                MessageBoxW(hwnd, L"Failed to save address book", L"Error", MB_OK);
            }
        }
    }
}
void LoadContact(HWND hwnd) {
    std::locale::global(std::locale("uk_UA.UTF-8"));
    std::wifstream infile;
    infile.open(L"address_book.txt");
    if (infile.is_open())
    {
        if (SendMessageW(hListBox, LB_GETCOUNT, 0, 0) != 0)
        {
            SendMessageW(hListBox, LB_RESETCONTENT, 0, 0);
        }
        bool fileIsEmpty = true;
        std::wstring line;
        while (std::getline(infile, line))
        {
            SendMessageW(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(line.c_str()));
            fileIsEmpty = false;
        }
        infile.close();
        if (fileIsEmpty) {
            std::wstring output = L"The address book is empty";
            SendMessageW(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(output.c_str()));
        }
    }
    else
    {
        MessageBoxW(hwnd, L"Failed to load address book", L"Error", MB_OK);
    }
}
void SearchContact() {
    std::locale::global(std::locale("uk_UA.UTF-8"));
    
    const int bufferSize = 100;
    wchar_t buffer[bufferSize];
    GetWindowTextW(hEditSearch, buffer, bufferSize);
    if (SendMessageW(hListBox, LB_GETCOUNT, 0, 0) != 0)
    {
        SendMessageW(hListBox, LB_RESETCONTENT, 0, 0);
    }
    // ���������, ��� ������ ������ �� �����
    std::wstring searchText(buffer);
    if (searchText.empty())
    {
        std::wstring output = L"Please enter a search term";
        SendMessageW(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(output.c_str()));
        return;
    }
    // ��������� ���� ��� ������
    std::wifstream infile;
    infile.open(L"address_book.txt");
    if (infile.is_open())
    {
        std::wstring line;
        std::wstring fullname, homeAddress, workAddress, phoneNumber, additionalInfo;
        bool found = false;
        while (std::getline(infile, line))
        {
            // ��������� ���������� �� ������
            if (line.find(L"Full Name: ") != std::string::npos)
            {
                fullname = line.substr(11);
            }
            else if (line.find(L"Home Address: ") != std::string::npos)
            {
                homeAddress = line.substr(14);
            }
            else if (line.find(L"Work Address: ") != std::string::npos)
            {
                workAddress = line.substr(14);
            }
            else if (line.find(L"Phone Number: ") != std::string::npos)
            {
                phoneNumber = line.substr(14);
            }
            else if (line.find(L"Additional Info: ") != std::string::npos)
            {
                additionalInfo = line.substr(17);
            }
            else if (line.find(L"----------------------------------------") != std::string::npos)
            {
                // ���������, ������������� �� ���������� ���������� �������
                if (fullname.find(searchText) != std::string::npos ||
                    homeAddress.find(searchText) != std::string::npos ||
                    workAddress.find(searchText) != std::string::npos ||
                    phoneNumber.find(searchText) != std::string::npos)
                {
                    SendMessageW(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>((L"Full Name: " + fullname).c_str()));
                    SendMessageW(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>((L"Home Address: " + homeAddress).c_str()));
                    SendMessageW(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>((L"Work Address: " + workAddress).c_str()));
                    SendMessageW(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>((L"Phone Number: " + phoneNumber).c_str()));
                    SendMessageW(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>((L"Additional Info: " + additionalInfo).c_str()));
                    std::wstring separator = L"----------------------------------------";
                    separator.push_back('\0');
                    SendMessageW(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(separator.c_str()));
                    found = true;
                }
                fullname = L"";
                homeAddress = L"";
                workAddress = L"";
                phoneNumber = L"";
                additionalInfo = L"";
            }
        }
        infile.close();

        if (found)
        {
            std::wstring output = L"Person found!";
            SendMessageW(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(output.c_str()));
        }
        if (!found)
        {
            std::wstring output = L"Nothing found for \"" + searchText + L"\"";
            SendMessageW(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(output.c_str()));
        }
    }
    else
    {
        std::wstring output = L"Unable to open file";
        SendMessageW(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(output.c_str()));
    }
}
void DeleteContact(HWND hwnd, int selectedIndex) {
    std::locale::global(std::locale("uk_UA.UTF-8"));
    if (selectedIndex == -1) {
        MessageBox(hwnd, L"Please select a contact to delete", L"Error", MB_ICONERROR);
        return;
    }
    int result = MessageBox(hwnd, L"Are you sure you want to delete the selected contact?", L"Confirmation", MB_YESNO | MB_ICONQUESTION);
    if (result != IDYES) {
        EndDialog(hwnd, IDCANCEL);
        return;
    }
    std::wifstream infile("address_book.txt");
    std::wstring line;
    // ������� ����� ���� ��� ������
    std::wofstream outfile("temp_address_book.txt");

    int index = 0;
    bool foundSelectedContact = false;
    // ������ ������ �� ������� ����� � ���������� �� � �����, ����� ��������� ��� ��������
    while (std::getline(infile, line)) {
        // ���� �� �������� ��������� ������ ��� ��������, �� ���������� ��� ������ �� ���������� ����������� "--------"
        if (index == selectedIndex) {
            // ���������, ��� ��������� ������ �������� ���������� � "Full Name"
            if (line.find(L"Full Name: ") == std::wstring::npos) {
                // ������� ��������� �� ������, ���� ��������� ������ �� �������� ���������� � "Full Name"
                MessageBoxW(hwnd, L"Please select a contact by Full Name for deletion", L"Error", MB_OK);
                infile.close();
                outfile.close();
                remove("temp_address_book.txt");  // ������� ��������� ����
                return;
            }
            foundSelectedContact = true;
            while (std::getline(infile, line)) {
                if (line.find(L"--------") != std::wstring::npos) {
                    break;
                }
            }
            index++;
            continue;
        }
        // ���� ������ �� ���� ������� ��� ��������, �� ���������� �� � ����� ����
        outfile << line << std::endl;
        index++;
    }
    infile.close();
    outfile.close();

    if (!foundSelectedContact) {
        // ������� ��������� �� ������, ���� ��������� ������� �� ��� ������
        MessageBoxW(hwnd, L"Selected contact not found", L"Error", MB_OK);
        remove("temp_address_book.txt");  // ������� ��������� ����
        return;
    }

    // ������� ������ ���� � ��������������� �����
    if (remove("address_book.txt") != 0) {
        // ������� ��������� �� ������, ���� �� ������� ������� ������ ����
        MessageBoxW(hwnd, L"Failed to delete contact from address book", L"Error", MB_OK);
    }
    else if (rename("temp_address_book.txt", "address_book.txt") != 0) {
        // ������� ��������� �� ������, ���� �� ������� ������������� ����� ����
        MessageBoxW(hwnd, L"Failed to delete contact from address book", L"Error", MB_OK);
    }
    else {
        LoadContact(hwnd);
        MessageBoxW(hwnd, L"Contact deleted successfully", L"Success", MB_OK);
    }
}
void SaveEditedContact(const std::wstring& originalText, const std::wstring& editedText)
{
    std::locale::global(std::locale("uk_UA.UTF-8"));
    std::wifstream infile("address_book.txt");
    std::wstring line;
    std::wstring updatedContent;
    

    while (std::getline(infile, line))
    {
        if (line == originalText)
        {
            updatedContent += editedText;
        }
        else
        {
            updatedContent += line;
        }
            updatedContent += L"\n";
    }

    infile.close();

    // ��������� ���� ��� ������
    std::wofstream outfile("address_book.txt");
    if (outfile.is_open())
    {
        // ���������� ����������� ���������� � ����
        outfile << updatedContent;
        outfile.close();
    }

    else
    {
        MessageBox(nullptr, L"Failed to save changes", L"Error", MB_OK);
    }
}
INT_PTR CALLBACK EditDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::locale::global(std::locale("uk_UA.UTF-8"));
    static std::wstring originalText;

    switch (msg)
    {
    case WM_INITDIALOG:
    {
        // ��������� ��������� ���������� �� ListBox
        int selectedIndex = static_cast<int>(lParam);
        int textLength = SendMessage(hListBox, LB_GETTEXTLEN, selectedIndex, 0);
        std::vector<wchar_t> buffer(textLength + 1);
        SendMessage(hListBox, LB_GETTEXT, selectedIndex, reinterpret_cast<LPARAM>(buffer.data()));
        originalText = buffer.data();

        // ����������� ������� ��������� � ������������ ����������
        size_t colonIndex = originalText.find(L":");
        if (colonIndex != std::wstring::npos)
        {
            // ��������� ������ ����� ���������
            std::wstring editedText = originalText.substr(colonIndex + 2);

            // ��������� ������ � ���� ��������������
            SetDlgItemTextW(hwnd, IDC_EDIT, editedText.c_str());
        }

        return INT_PTR(TRUE);
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_BUTTON_SAVE_EDITED:
        {
            
            int result = MessageBox(hwnd, L"Do you want to save the changes?", L"Confirmation", MB_YESNO | MB_ICONQUESTION);
            if (result != IDYES) {
                EndDialog(hwnd, IDCANCEL);
                return INT_PTR(TRUE);
            }
            else {
                 MessageBox(hwnd, L"Contact edit successfully", L"Success", MB_OK);
            }
            // ��������� ���������� ���������� �� ����������� ����
            const int bufferSize = 100;
            wchar_t buffer[bufferSize];
            GetDlgItemTextW(hwnd, IDC_EDIT, buffer, bufferSize);
            std::wstring editedText = buffer;
            // ������������ ������ � ����������� ����������� � ������� "Full Name: ���������� ����������"
            std::wstring savedText = originalText.substr(0, originalText.find(L":") + 2) + editedText;
            // ������ ������������ ���������� � ListBox �� ����������������� ����������
            int selectedIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
            SendMessage(hListBox, LB_DELETESTRING, selectedIndex, 0);
            SendMessage(hListBox, LB_INSERTSTRING, selectedIndex, reinterpret_cast<LPARAM>(savedText.c_str()));

            // ���������� ���������� ���������� � �����
            SaveEditedContact(originalText, savedText);

            EndDialog(hwnd, IDOK);
            return INT_PTR(TRUE);
        }

        case IDCANCEL:
        {
            EndDialog(hwnd, IDCANCEL);
            return INT_PTR(TRUE);
        }
        }
        break;
    }
    }
    return INT_PTR(FALSE);
}
void OpenEditDialog(HWND hwnd, int selectedIndex)
{
    std::locale::global(std::locale("uk_UA.UTF-8"));
    HWND hEditDialog = CreateDialogParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_EDIT_DIALOG),hwnd, EditDialogProc, selectedIndex);

    if (hEditDialog)
    {
        // ��������� ���������� ������� � ���������������� ������ ����������� ����
        SetWindowLongPtr(hEditDialog, GWLP_USERDATA, selectedIndex);
        ShowWindow(hEditDialog, SW_SHOW);
    }
    else
    {
        MessageBox(hwnd, L"Failed to open dialog box.", L"Error", MB_ICONERROR);
    }
}
bool UkrainianLastNameComparator(const std::wstring& lastName1, const std::wstring& lastName2) {
    std::locale ukrainianLocale("uk_UA.UTF-8");
    return std::use_facet<std::collate<wchar_t>>(ukrainianLocale).compare(lastName1.data(), lastName1.data() + lastName1.length(),
        lastName2.data(), lastName2.data() + lastName2.length()) < 0;
}
void SortContactsByLastName(HWND hwnd) {
    std::locale::global(std::locale("uk_UA.UTF-8"));
    std::wifstream infile("address_book.txt");
    if (infile.is_open()) {
        std::vector<std::wstring> contacts;
        std::wstring line;
        while (std::getline(infile, line)) {
            if (line.find(L"Full Name: ") != std::wstring::npos) {
                std::wstring fullName = line.substr(11);
                contacts.push_back(fullName);
            }
        }
        infile.close();
        // ��������� �������� �� ������� � ������� ����������������� �����������
        std::sort(contacts.begin(), contacts.end(), UkrainianLastNameComparator);
        std::wofstream outfile("sorted_address_book.txt");
        if (outfile.is_open()) {
            for (const std::wstring& contact : contacts) {
                // ���� ������� � �������� ����� � ���������� ��� ��� ������ � ����� ����
                infile.open("address_book.txt");
                if (infile.is_open()) {
                    bool contactFound = false;
                    while (std::getline(infile, line)) {
                        if (line.find(L"Full Name: ") != std::wstring::npos && line.substr(11) == contact) {
                            contactFound = true;
                        }
                        if (contactFound) {
                            outfile << line << std::endl;
                            if (line.find(L"----------------------------------------") != std::wstring::npos) {
                                break;
                            }
                        }
                    }
                    infile.close();
                }
            }
            outfile.close();

            if (std::remove("address_book.txt") == 0 && std::rename("sorted_address_book.txt", "address_book.txt") == 0) {
                 MessageBox(NULL, L"Contacts sorted by last name!", L"Success", MB_OK | MB_ICONINFORMATION);
                 LoadContact(hwnd);
            }
            else {
                 MessageBox(NULL, L"Failed to sort contacts!", L"Error", MB_OK | MB_ICONERROR);
            }
        }
        else {
            MessageBox(NULL, L"Failed to create sorted address book file!", L"Error", MB_OK | MB_ICONERROR);
        }
    }
    else {
        MessageBox(NULL, L"Failed to open address book file!", L"Error", MB_OK | MB_ICONERROR);
    }
}




