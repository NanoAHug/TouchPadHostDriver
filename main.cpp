#include <windows.h>
#include <iostream>
#include <string>

#define PORT_NAME "COM4"    // 替换为你的 Pico 虚拟串口名（如 COM3）
#define BAUD_RATE CBR_115200
#define BUFFER_SIZE 256

void processLine(const std::string& line) {
    if (line.rfind("[Touch]", 0) == 0) {
        int x, y;
        if (std::sscanf(line.c_str(), "[Touch]/x:%d/y:%d", &x, &y) == 2) {
            std::cout << "Parsed x: " << x << ", y: " << y << std::endl;
        } else {
            std::cout << "Failed to parse line: " << line << std::endl;
        }
    }
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hSerial;
    DWORD bytesRead;
    char buffer[BUFFER_SIZE];
    std::string currentLine;

    // 打开串口设备
    hSerial = CreateFileA(PORT_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                          OPEN_EXISTING, 0, NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "[错误] 无法打开串口 " << PORT_NAME << "\n";
        return 1;
    }

    // 设置串口参数
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);

    dcbSerialParams.BaudRate = BAUD_RATE;   // 波特率
    dcbSerialParams.ByteSize = 8;           // 8位数据位
    dcbSerialParams.StopBits = ONESTOPBIT;  // 1个停止位
    dcbSerialParams.Parity   = NOPARITY;    // 无奇偶校验

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "[错误] 设置串口参数失败\n";
        CloseHandle(hSerial);
        return 1;
    }

    // 设置串口超时（可选）
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;           // 读字符间隔超时
    timeouts.ReadTotalTimeoutConstant = 50;      // 读操作总超时
    timeouts.ReadTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);

    std::cout << "[信息] 串口已连接，监听中...\n";

    // 主循环：持续读取串口数据
    while (true) {
        char ch;
        if (ReadFile(hSerial, &ch, 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                if (ch == '\r' || ch == '\n') {
                    if (!currentLine.empty()) {
                        processLine(currentLine);
                        currentLine.clear();
                    }
                }
                else {
                    currentLine += ch;
                }
            }
        }
        else {
            std::cerr << "[错误] 串口读取失败\n";
            break;
        }
    }

    CloseHandle(hSerial);
    return 0;
}
