#include <iostream>
#include <QApplication>
#include "ui/MainWindow.h"
#include <iomanip> // for std::hex, std::setw, std::setfill
#include <Windows.h>

int main(int argc, char** argv) {
    SetConsoleOutputCP(CP_UTF8); // 设置控制台输出代码页为 UTF-8
    // std::cout << "Hello, World!" << std::endl;

    QApplication app(argc, argv);

    MainWindow w;
    w.show();
    
    return app.exec();
}