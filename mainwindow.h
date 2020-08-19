#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ChildWnd;
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
    void refreshMenus();//刷新菜单
    void addSubWdnList();//添加子窗口列表
private:
    void FormatSetEnable();
    ChildWnd* activateChildWnd();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
