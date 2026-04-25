#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

private slots:
    void on_touchPushed_clicked();

private:
    Ui::Window *ui;
    std::uint32_t counter{};
};

#endif // WINDOW_H
