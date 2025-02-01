#include "dialog_dynamic_03.h"

#include <QApplication>

#include <string>
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) 
{
    std::cout << "start guile 04_03" << std::endl;

    QApplication app(argc, argv);

    std::cout << "QApplication started" << std::endl;
    
    auto dialog_03{start_dynamic_dialog_03()};

    return app.exec();
}
