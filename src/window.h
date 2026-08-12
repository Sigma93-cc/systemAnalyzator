#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <string>
namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr, std::string_view osver = {}, std::string_view softver = {});
    ~Window();

private:
    Ui::Window *ui;
};

#endif // WINDOW_H
