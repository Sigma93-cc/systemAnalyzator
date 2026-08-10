#include <iostream>
#include <QApplication>
#include "window.h"
#include <spdlog/spdlog.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Window w;
    w.show();
    return app.exec();
}
