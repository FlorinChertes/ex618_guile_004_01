#include "dialog_dynamic.h"

#include <QApplication>

#include <string>
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) 
{
    std::cout << "start guile 04_01" << std::endl;

    QApplication app(argc, argv);

    std::cout << "QApplication started" << std::endl;
    
    auto dialog_01{start_dynamic_dialog_01()};

    return app.exec();
}
