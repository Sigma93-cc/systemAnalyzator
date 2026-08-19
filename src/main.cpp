#include <iostream>
#include <QApplication>
#include "window.h"
#include "versionchecker.h"
#include <version.h>
#include "ubootmanager.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    auto[osver, softver] = Versionchecker::getInfo();
    auto[bootpart,up_available] = Ubootmanager::instance().info();
    std::cout << "Run application with OS: " << osver << " softver: " << softver << std::endl;
    std::cout << "Active part: " << bootpart << " upgraded: " << up_available << std::endl;
    Ubootmanager::instance().mark_good();
    Window w(nullptr, osver, softver);
    if (notfullscreen)
        w.show();

    w.showFullScreen();

    return app.exec();
}
