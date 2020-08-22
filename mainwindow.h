#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QMdiSubWindow>

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
    void docOpen();
    QMdiSubWindow* findChildWnd(const QString& docName);
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void on_newAction_triggered();
    void refreshMenus();//刷新菜单
    void addSubWdnList();//添加子窗口列表
    void on_closeAction_triggered();

    void on_closeAllAction_triggered();

    void on_titleAction_triggered();

    void on_casecadeAction_triggered();

    void on_nextAction_triggered();

    void on_previousAction_triggered();
    void setSubActiveWindow(QWidget*);
    void on_openAction_triggered();

private:
    void FormatSetEnable();
    ChildWnd* activateChildWnd();
private:
    Ui::MainWindow *ui;
    QSignalMapper *m_sigalMapper;
};

#endif // MAINWINDOW_H
