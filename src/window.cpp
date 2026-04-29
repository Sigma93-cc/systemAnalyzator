#include "window.h"
#include "ui_window.h"
#include "QDebug"

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
}

Window::~Window()
{
    delete ui;
}

void Window::on_touchPushed_clicked()
{
    qDebug() << "Touch clicked!!!";
    ui->counter->setText(QString::number(++counter));
}

