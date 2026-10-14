#include "window.h"
#include "ui_window.h"
#include "QDebug"
#include <QMessageBox>
#include <QTimer>

Window::Window(QWidget *parent, std::string_view osver, std::string_view softver) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);    
    ui->softversion->setText("Soft: " + QString::fromStdString(std::string(softver)));
    ui->osversion->setText("OS: " + QString::fromStdString(std::string(osver)));

    box = new QMessageBox(this);
    box->setStyleSheet(
        "QWidget { background-color: rgb(40,40,40); color: white; font-size: 16px; }"
        "QPushButton { color: white; border: 1px solid white; padding: 6px 16px; min-height: 40px; }");
    box->setStandardButtons(QMessageBox::Cancel);
    box->setDefaultButton(QMessageBox::Cancel);
    box->setButtonText(QMessageBox::Cancel, "Отмена");
    box->setText("Установлено новое обновление\n перезагрузка через (60)");

    ticker = new QTimer(this);
    ticker->setInterval(1000);
    ticker->setSingleShot(false);
    QObject::connect(ticker, &QTimer::timeout, this, &Window::on_timeout);
    QObject::connect(box, &QMessageBox::buttonClicked, this, &Window::button_cncl_clicked);
}

Window::~Window()
{
    delete ui;
}

void Window::on_timeout()
{
    --answer_reboot_counter;
    if (answer_reboot_counter != 0)
        box->setText("Установлено новое обновление\n перезагрузка через (" + QString::number(answer_reboot_counter) + ")");
    else {
        ticker->stop();
        box->close();
        emit needReboot();
    }
}

void Window::button_cncl_clicked(QAbstractButton* btn)
{
    Q_UNUSED(btn);
    ticker->stop();
    emit rebootCanceled();
}

void Window::showSystemStatus(bool ok)
{
    if (!ok) {
        ui->label->setText("СИСТЕМА НЕИСПРАВНА");
        ui->label->setStyleSheet("color: rgb(255, 0, 0)");
    } else {
        ui->label->setText("СИСТЕМА ИСПРАВНА");
        ui->label->setStyleSheet("color: rgb(0, 255, 0)");
    }
}

void Window::answer_reboot()
{
    answer_reboot_counter = 60;
    ticker->start();
    box->show();
}

void Window::stop_answer_reboot()
{
    box->close();
    ticker->stop();
}

bool Window::reboot_answered()
{
    return  box->isVisible();
}
