#pragma once

#include <QMainWindow>
#include <string>
#include <QMessageBox>
#include <QTimer>
#include <QAbstractButton>
namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr, std::string_view osver = {}, std::string_view softver = {});
    ~Window();
    void answer_reboot();
    void stop_answer_reboot();
    bool reboot_answered();
public slots:
    void showSystemStatus(bool ok = true);
signals:
    void needReboot();
    void rebootCanceled();
private slots:
    void on_timeout();
    void button_cncl_clicked(QAbstractButton* btn);
private:
    std::uint32_t answer_reboot_counter = 60;
    Ui::Window *ui;
    QMessageBox *box;
    QTimer* ticker;
};
