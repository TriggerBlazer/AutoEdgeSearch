#include <iostream>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <random>
#include <codecvt>

using namespace std;

string url_encode(const string& str) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex << std::uppercase;

    for (char c : str) {
        escaped << '%' << std::setw(2)
            << static_cast<int>(static_cast<unsigned char>(c));
    }

    return escaped.str();
}


wstring generate_random_chinese(int length) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0x4E00, 0x9FA5);
    wstring str;

    for (int i = 0; i < length; ++i) {
        str.push_back(static_cast<wchar_t>(dist(gen)));
    }
    return str;
}

std::string wstringToUtf8(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.to_bytes(str);
}

std::wstring utf8ToWstring(const std::string& str)
{
    std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.from_bytes(str);
}

void searchInEdge(const std::string& searchStr) {
    std::string encodedSearchStr;
    for (char c : searchStr) {
        if (isalnum(c)) {
            encodedSearchStr += c;
        }
        else {
            char buffer[4];
            sprintf_s(buffer, "%%%02X", (unsigned char)c);
            encodedSearchStr += buffer;
        }
    }

    std::string url = "https://www.bing.com/search?q=" + encodedSearchStr;

    std::string command = "start ms-edge://" + url;
    system(command.c_str());
}

// Generate a random hexadecimal string (for cvid parameters)
std::string GenerateHexString(int length) {
    static const char hex[] = "0123456789ABCDEF";
    std::string result;
    std::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<> dis(0, 15);
    for (int i = 0; i < length; ++i) {
        result += hex[dis(gen)];
    }
    return result;
}

#include <TlHelp32.h>
void CloseEdgeProcess() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32W processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(snapshot, &processEntry)) {
        do {
            std::wstring processName(processEntry.szExeFile);
            if (processName == L"msedge.exe") {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processEntry.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                }
            }
        } while (Process32NextW(snapshot, &processEntry));
    }
    CloseHandle(snapshot);
}

int main(int argc, char* argv[]) {
    // auto edgeExexUrl = "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe";
     // 初始化随机种子
    srand(time(nullptr));
    //random_device rd;

    int excuteTimes = 63;
    for (int i = 0; i < excuteTimes; ++i) 
    {

        // Delay by 5 seconds
        Sleep(10000);

        auto length = rand() % 6 + 5;
        if (argc > 1)
        {
            length = atoi(argv[1]);
        }
        wstring chinese_str = generate_random_chinese(length);

        // Convert to UTF-8 and encode the URL
        string utf8_str = wstringToUtf8(chinese_str);
        string encoded_str = url_encode(utf8_str);

        // create search URL
        string url = "https://www.bing.com/search?q="
            + encoded_str
            + "&form=QBLH" 
            + "&cvid=" + GenerateHexString(8);

        // Convert to a wide string
        wstring wide_url = utf8ToWstring(url);

        // Open it using the Edge browser
        HINSTANCE result = ShellExecuteW(
            NULL,
            L"open",
            L"msedge.exe",
            wide_url.c_str(),
            NULL,
            SW_SHOW
        );

        if ((int)result <= 32) {
            cerr << "The Edge browser cannot be opened. Error code:" << (int)result << endl;
            return 1;
        }

        wcout << "Generated search content" << chinese_str << endl;
    }

    CloseEdgeProcess();
    return 0;
}