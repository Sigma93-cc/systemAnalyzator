#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QTimer>
#include "window.h"

class UbootEnvManager;
class Core : public QObject
{
    enum class PartMode {
        trial,    
        normal,
        newsoft,
        fatal_error
    };

    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);
    ~Core();
public slots:
    void on_reboot();
    void on_reboot_canceled();
private slots:
    void on_markgood();
    void on_monitoring();
private:
    std::string start_part{};
    void init();
    void on_setup_markgood();
    void on_setup_monitoring();
    void on_swupdate_progress();
    bool check_swupdate();
    PartMode partition_mode();
    std::string active_part();
    std::unique_ptr<Window> window;
    std::unique_ptr<QTimer> pending_timer;
    std::unique_ptr<UbootEnvManager> uboot_env;
};

#endif // CORE_H
