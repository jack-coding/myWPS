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
    Q_OBJECT//使用元对象系统
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initMainWindow();//初始化窗口
    void docNew();//新建文档
    void docOpen();//打开文档
    void docSave();//保存文档
    void docSaveAs();//另存文档
    void docUndo();//撤销操作
    void docRedo();//重做操作
    void docCut();//剪切
    void docCopy();//复制
    void docPaste();//粘贴
    void textBold();//字体加粗
    void textItalic();//字体倾斜
    void textUnderline();//字体下划线
    void textFamily(const QString& family);//字体类型
    void textSize(const QString& fontSize);//字号
    void textColor();//字体颜色
    QMdiSubWindow* findChildWnd(const QString& docName);//根据文档名查找子窗口
protected:
    void closeEvent(QCloseEvent *event);//重写关闭事件
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
    void on_saveAction_triggered();
    void on_otherSaveAction_triggered();
    void on_undoAction_triggered();
    void on_redoAction_triggered();
    void on_cutAction_triggered();
    void on_copyAction_triggered();
    void on_pasteAction_triggered();
    void on_boldAction_triggered();
    void on_italicAction_triggered();
    void on_underlineAction_triggered();
    void on_fontComboBox_activated(const QString &arg1);
    void on_fontSizeComBox_activated(const QString &arg1);
    void on_leftAlignAction_triggered();
    void on_centerAction_triggered();
    void on_rightAlighAction_triggered();
    void on_justifyAction_triggered();
    void on_colorAction_triggered();

private:
    void FormatSetEnable();
    ChildWnd* activateChildWnd();
private:
    Ui::MainWindow *ui;
    QSignalMapper *m_sigalMapper;
};

#endif // MAINWINDOW_H
