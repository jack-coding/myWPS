#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initMainWindow();
    void docNew();
private slots:
    void on_newAction_triggered();

private:
    void FormatSetEnable();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
