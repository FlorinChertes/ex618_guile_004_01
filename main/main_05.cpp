#include "dialog_dynamic_05.h"

#include <QApplication>

#include <string>
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) 
{
    std::cout << "start guile 04_05" << std::endl;

    QApplication app(argc, argv);

    std::cout << "QApplication started" << std::endl;
    
    auto dialog_{start_dynamic_dialog()};

    return app.exec();
}
