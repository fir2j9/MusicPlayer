#include <iostream>
#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char** argv) {
    // std::cout << "Hello, World!" << std::endl;
    QApplication app(argc, argv);

    MainWindow w;
    w.show();
    
    return app.exec();
}