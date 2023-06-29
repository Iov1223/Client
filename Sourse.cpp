#include <iostream>
#include <fstream>
#include <Windows.h>
#include <Psapi.h>
#include <string>

std::string getCurrentDateTime()
{
    time_t now = time(nullptr);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y.%m.%d %H:%M:%S", &timeInfo);
    return std::string(buffer);
}

void WriteProcessToLog(const std::string& processName, const std::string& time)
{
    std::ofstream logFile("activity_log.txt", std::ios::app);
    if (logFile.is_open())
    {
        logFile << processName << "  " << getCurrentDateTime() << std::endl;
        logFile.close();
    }
    else
    {
        std::cout << "Ошибка открытия файла." << std::endl;
    }
}

std::string GetProcessNameFromHWND(HWND windowHandle)
{
    DWORD processId;
    GetWindowThreadProcessId(windowHandle, &processId);

    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (processHandle)
    {
        char processPath[MAX_PATH];
        if (GetModuleFileNameExA(processHandle, NULL, processPath, MAX_PATH))
        {
            char processName[MAX_PATH];
            char* processNameStart = strrchr(processPath, '\\');
            if (processNameStart)
            {
                strcpy_s(processName, sizeof(processName), processNameStart + 1);
            }
            else
            {
                strcpy_s(processName, sizeof(processName), processPath);
            }

            CloseHandle(processHandle);
            return std::string(processName);
        }

        CloseHandle(processHandle);
    }

    return "";
}

int main()
{
    while (true)
    {
        HWND foregroundWindow = GetForegroundWindow();
        std::string currentProcess = GetProcessNameFromHWND(foregroundWindow);

        WriteProcessToLog(currentProcess, getCurrentDateTime());

        Sleep(1000); // Задержка в 1 секунду
    }

    return 0;
}














//#include <iostream>
//#include <fstream>
//#include <Windows.h>
//#include <Psapi.h>
//
//std::string getCurrentDateTime()
//{
//    time_t now = time(nullptr);
//    struct tm timeInfo;
//    localtime_s(&timeInfo, &now);
//    char buffer[20];
//    strftime(buffer, sizeof(buffer), "%Y.%m.%d %H:%M:%S", &timeInfo);
//    return std::string(buffer);
//}
//HWND activeProcess() {
//    // Получение дескриптора активного окна
//    HWND foregroundWindow = GetForegroundWindow();
//
//    // Получение идентификатора процесса активного окна
//    DWORD processId;
//    GetWindowThreadProcessId(foregroundWindow, &processId);
//
//    // Получение дескриптора процесса
//    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
//
//    // Получение полного пути к исполняемому файлу процесса
//    char processPath[MAX_PATH];
//    if (GetModuleFileNameExA(processHandle, NULL, processPath, MAX_PATH))
//    {
//        // Получение названия процесса из полного пути
//        char processName[MAX_PATH];
//        char* processNameStart = strrchr(processPath, '\\');
//        if (processNameStart)
//        {
//            strcpy_s(processName, sizeof(processName), processNameStart + 1);
//        }
//        else
//        {
//            strcpy_s(processName, sizeof(processName), processPath);
//        }
//
//        // Закрытие дескриптора процесса
//        CloseHandle(processHandle);
//        return foregroundWindow;
//    }
//    //else
//    //{
//    //    // Закрытие дескриптора процесса
//    //    CloseHandle(processHandle);
//    //    return "Ошибка при получении информации о процессе.";
//    //}
//
//    
//}
//void writeActivityPeriodToFile(const std::string& startTime, const std::string& endTime)
//{
//    std::ofstream file("activity_log.txt", std::ios::app);
//    if (file.is_open())
//    {
//        file << "Процесс: " << activeProcess() << "; Период активности: " << startTime << " - " << endTime << std::endl;
//        file.close();
//    }
//    else
//    {
//        std::cout << "Ошибка открытия файла." << std::endl;
//    }
//}
//
//
//
//int main()
//{
//    setlocale(LC_ALL, "ru");
//
//    /*while (true)
//    {
//        std::cout << ActiveProcess();
//        system("cls");
//    }*/
//
//
//    HWND consoleWindow = GetConsoleWindow();
//    // Получение дескриптора текущего процесса
//    HANDLE currentProcess = GetCurrentProcess();
//    DWORD_PTR previousSetting;
//    DWORD_PTR processAffinityMask = 0;
//    if (GetProcessAffinityMask(currentProcess, &processAffinityMask, &previousSetting))
//    {
//        DWORD_PTR newSetting = processAffinityMask & ~(static_cast<DWORD_PTR>(1) << (sizeof(DWORD_PTR) * 8 - 1));
//        SetProcessAffinityMask(currentProcess, newSetting);
//    }
//
//    bool isActive = false;
//    std::string activeStartTime;
//    std::string activeEndTime;
//
//
//    while (true)
//    {
//        if (GetForegroundWindow() == consoleWindow)
//        {
//            if (!isActive)
//            {
//                isActive = true;
//                activeStartTime = getCurrentDateTime();
//            }
//        }
//        else
//        {
//            if (isActive)
//            {
//                isActive = false;
//                activeEndTime = getCurrentDateTime();
//                writeActivityPeriodToFile(activeStartTime, activeEndTime);
//            }
//        }
//
//        Sleep(1000);
//    }
//
//    return 0;
//}
