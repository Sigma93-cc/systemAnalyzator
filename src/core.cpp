#include "core.h"
#include <iostream>
#include <version.h>
#include "versionchecker.h"
#include "ubootmanager.h"
#include <QFile>
#include <QProcess>

Core::Core(QObject *parent)
    : QObject{parent}
{
    init();
}

Core::~Core()
{

}

void Core::init()
{
    auto[osver, softver] = Versionchecker::getInfo();
    std::cout << "Run application with OS: " << osver << " softver: " << softver << std::endl;

    window = std::make_unique<Window>(nullptr, osver, softver);
    QObject::connect(window.get(),&Window::needReboot, this, &Core::on_reboot);
    QObject::connect(window.get(),&Window::rebootCanceled, this, &Core::on_reboot_canceled);

    uboot_env = std::make_unique<UbootEnvManager>();
    pending_timer = std::make_unique<QTimer>();
    start_part = active_part();
    auto systemOk = true;

    if (notfullscreen)
        window->show();
    else
        window->showFullScreen();

    switch (partition_mode())
    {
        case Core::PartMode::trial:
            on_setup_markgood();
            break;
        case Core::PartMode::normal:
            on_setup_monitoring();
            window->showSystemStatus(systemOk);
            break;
        case Core::PartMode::fatal_error:
            window->showSystemStatus(!systemOk);
            break;
        case Core::PartMode::newsoft:
            on_setup_monitoring();
            window->showSystemStatus(systemOk);
            window->answer_reboot();
            break;
        default:
            return;
    }
}

void Core::on_setup_markgood()
{
    pending_timer->setInterval(30 * 1000);
    pending_timer->setSingleShot(true);
    pending_timer->start();
    QObject::connect(pending_timer.get(), &QTimer::timeout, this, &Core::on_markgood);
}
void Core::on_setup_monitoring()
{
    pending_timer->setInterval(5 * 1000);
    pending_timer->setSingleShot(false);
    pending_timer->start();
    QObject::connect(pending_timer.get(), &QTimer::timeout, this, &Core::on_monitoring);
}

void Core::on_markgood()
{
    std::cout << "Mark good timeout completed, setting systemstate to good" << std::endl;
    bool res = uboot_env->set("upgrade_available", "0") & uboot_env->set("bootcount", "0");
    std::cout << "Setting Uboot env values completed with res = " << std::boolalpha << res << std::endl;
    window->showSystemStatus(res);
    QObject::disconnect(pending_timer.get(), &QTimer::timeout, this, &Core::on_markgood);
    if (res) {
        on_setup_monitoring();
    } else {
        std::cerr << "Fatal error, u-boot env corrupted" << std::endl;
    }
}

bool Core::check_swupdate()
{
    return uboot_env->get("recovery_status") == "in_progress";
}

void Core::on_swupdate_progress()
{
    if (check_swupdate()) {
        std::cout << "Detect swupdate installing, setup monitoring" << std::endl;
        QObject::disconnect(pending_timer.get(), &QTimer::timeout, this, &Core::on_swupdate_progress);
        on_setup_monitoring();
    }
}

void Core::on_monitoring()
{
    //swupdate is installing new soft
    if (check_swupdate())  {
        std::cout << "Detected swupdate installing, stop reboot answering" << std::endl;
        if (window->reboot_answered())
            window->stop_answer_reboot();
        return;
    }

    if (partition_mode() == Core::PartMode::newsoft) {
        if (!window->reboot_answered())
            window->answer_reboot();
    }
}

std::string Core::active_part()
{
    std::string res = "undef";
    QFile f(qEnvironmentVariable("SA_CMDLINE", "/proc/cmdline"));
    if (!f.open(QIODevice::ReadOnly))
        return res;
    for (const QByteArray &arg : f.readAll().simplified().split(' ')) {
        if (arg.startsWith("root=/dev/mmcblk0p")) {
            res = arg.mid(18).toStdString();   // 18 = длина "root=/dev/mmcblk0p"
        }
    }
    return res;
}

Core::PartMode Core::partition_mode()
{
    auto up_available = uboot_env->get("upgrade_available");
    if (up_available.empty())
        return Core::PartMode::fatal_error;

    auto bootpart = uboot_env->get("bootpart");
    if (bootpart.empty())
        return Core::PartMode::fatal_error;

    if (start_part == "undef")
        return Core::PartMode::fatal_error;


    std::cout << "Current configuration: system runned on part : " << start_part << " upgrade = " << up_available << " next bootpart = " << bootpart << std::endl;
    if (up_available == "1") {
        if (bootpart == start_part) {
            return Core::PartMode::trial;
        } else {
            return Core::PartMode::newsoft;
        }
    }

    return Core::PartMode::normal;
}


void Core::on_reboot()
{
    //swupdate is installing new soft
    if (check_swupdate()) {
        std::cout << "Detected swupdate installing, stop system rebooting" << std::endl;
        return;
    }
    std::cout << "Calling reboot with systemd" << std::endl;
    QProcess::startDetached("systemctl", {"reboot"});
}

void Core::on_reboot_canceled()
{
    QObject::disconnect(pending_timer.get(), &QTimer::timeout, this, &Core::on_monitoring);
    QObject::connect(pending_timer.get(), &QTimer::timeout, this, &Core::on_swupdate_progress);
    std::cout << "User has canceled reboot" << std::endl;
}
