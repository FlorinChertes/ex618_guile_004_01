#include "dialog_dynamic_02.h"

#include <QApplication>

#include <string>
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) 
{
    std::cout << "start guile 04_02" << std::endl;

    QApplication app(argc, argv);

    std::cout << "QApplication started" << std::endl;
    
    auto dialog_02{start_dynamic_dialog_02()};

    return app.exec();
}
