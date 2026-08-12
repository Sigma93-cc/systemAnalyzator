#include "window.h"
#include "ui_window.h"
#include "QDebug"

Window::Window(QWidget *parent, std::string_view osver, std::string_view softver) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);    
    ui->softversion->setText("Soft: " + QString::fromStdString(std::string(softver)));
    ui->osversion->setText("OS: " + QString::fromStdString(std::string(osver)));
}

Window::~Window()
{
    delete ui;
}

