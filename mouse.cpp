//
// Created by 13776 on 2025/5/19.
//

#include "mouse.h"
void MouseHandler(int x,int y) {

    // 获取屏幕分辨率
    int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 计算正方形区域边长（取较短边以适配所有屏幕）
    int squareSize = (screenWidth < screenHeight) ? screenWidth : screenHeight;

    // 中心点坐标
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    // 左下角坐标（以中心为基准）
    int left   = centerX - squareSize / 2;
    int bottom = centerY + squareSize / 2;  // 注意 Windows Y轴向下

    // 每块大小
    int blockSize = squareSize / 6;

    // 注意：y=0 是左下角，所以需要翻转 y 方向
    int targetX = left + blockSize * x + blockSize / 2;
    int targetY = bottom - blockSize * (y + 1) + blockSize / 2;

    // 将坐标转换为 SendInput 需要的 0-65535 绝对坐标
    int dx = (targetX * 65535) / screenWidth;
    int dy = (targetY * 65535) / screenHeight;



    INPUT input = {0};

    input.mi.dx = dx;
    input.mi.dy = dy;
    input.type = INPUT_MOUSE;

    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    SendInput(1, &input, sizeof(INPUT));


    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;


    SendInput(1, &input, sizeof(INPUT));

    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;


    SendInput(1, &input, sizeof(INPUT));
}