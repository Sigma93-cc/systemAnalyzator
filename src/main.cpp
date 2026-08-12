#include <iostream>
#include <QApplication>
#include "window.h"
#include "versionchecker.h"
#include <version.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    auto[osver, softver] = Versionchecker::getInfo();
    std::cout << "Run application with OS: " << osver << " softver: " << softver << std::endl;

    Window w(nullptr, osver, softver);
    if (notfullscreen)
        w.show();

    w.showFullScreen();
    return app.exec();
}
