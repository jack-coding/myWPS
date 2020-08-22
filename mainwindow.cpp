#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFont>
#include <QApplication>
#include <QMdiSubWindow>
#include "childwnd.h"
#include<QCloseEvent>
#include<QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindow()
{
    QFontDatabase fontDb;//字体数据库

    //将所有字体的字号添加到字体下拉列表中
    QList<int> fontSize=fontDb.standardSizes();
    foreach(int size,fontSize)
        ui->fontSizeComBox->addItem(QString::number(size));

    //获取当前应用程序的字号
    QFont defFont=QApplication::font();
    int defFontSize=defFont.pointSize();

    //找到该字号在下拉列表中的下标
    int defIndex=ui->fontSizeComBox->findText(QString("%1").arg(defFontSize));

    //将应用程序默认字号设置为要显示的字号
    ui->fontComboBox->setCurrentIndex(defIndex);

    //添加水平以及垂直滚动条
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    addSubWdnList();//添加子窗口列表
    connect(ui->mdiArea,&QMdiArea::subWindowActivated,this,&MainWindow::refreshMenus);
    connect(ui->menu_W,&QMenu::aboutToShow,this,&MainWindow::addSubWdnList);
    //创建信号映射器
    m_sigalMapper=new QSignalMapper;
    connect(m_sigalMapper,SIGNAL(mapped(QWidget*)),this,SLOT(setSubActiveWindow(QWidget*)));//将信号映射器的mapped信号与设置活动子窗口方法建立关联
}

void MainWindow::docNew()
{
    ChildWnd* childWnd=new ChildWnd;
    ui->mdiArea->addSubWindow(childWnd);

    connect(childWnd, SIGNAL(copyAvailable(bool)),ui->copyAction,SLOT(setEnabled(bool)));
    connect(childWnd,SIGNAL(copyAvailable(bool)),ui->cutAction,SLOT(setEnabled(bool)));

    childWnd->newDoc();
    childWnd->show();

    FormatSetEnable();
}


void MainWindow::docOpen()
{
    QString docName=QFileDialog::getOpenFileName(this,"打开文档","","txt文件(*.txt);;""HTML文件(*.html);;""所有文件(*.*)");
    if(!docName.isEmpty()){
        QMdiSubWindow* subwindow=findChildWnd(docName);
        if(subwindow){
            setSubActiveWindow(subwindow);
        }
        else{
            ChildWnd * childwnd=new ChildWnd;
            ui->mdiArea->addSubWindow(childwnd);//添加新的子窗口
            connect(childwnd,SIGNAL(copyAvailable(bool)),ui->cutAction,SLOT(setEnabled(bool)));
            connect(childwnd,SIGNAL(copyAvailable(bool)),ui->copyAction,SLOT(setEnabled(bool)));
            if(childwnd->loadFile(docName)){
                statusBar()->showMessage("文档已打开",3000);
                childwnd->show();
                FormatSetEnable();
            }
            else{
                childwnd->close();
            }
        }
    }
}

QMdiSubWindow *MainWindow::findChildWnd(const QString& docName)//在子窗口列表中寻找文件名为docName的窗口
{
    QString filePath=QFileInfo(docName).canonicalFilePath();
    foreach (QMdiSubWindow* subWindow, ui->mdiArea->subWindowList()) {
       ChildWnd* childWnd=qobject_cast<ChildWnd *> (subWindow->widget());
       if(childWnd->m_curfilePath==docName)
           return subWindow;
    }
    return 0;
}

void MainWindow::closeEvent(QCloseEvent *event)//重写关闭事件,一旦关闭主窗口,将所有子窗口关闭
{
    ui->mdiArea->closeAllSubWindows();//将所有子窗口关闭
    if(ui->mdiArea->currentSubWindow()){//如果还有子窗口未关闭,则将关闭事件忽略
        event->ignore();
    }
    else{//如果已经关闭所有子窗口,则将close事件接受
        event->accept();
    }
}

void MainWindow::FormatSetEnable()//将风格控制菜单项激活使能状态
{
    ui->boldAction->setEnabled(true);
    ui->centerAction->setEnabled(true);
    ui->leftAlignAction->setEnabled(true);
    ui->rightAlighAction->setEnabled(true);
    ui->italicAction->setEnabled(true);
    ui->underlineAction->setEnabled(true);
    ui->justifyAction->setEnabled(true);
}

ChildWnd *MainWindow::activateChildWnd()
{
    QMdiSubWindow* subActivateWnd= ui->mdiArea->activeSubWindow();
    if(subActivateWnd){
        ChildWnd* subActivateCWnd=qobject_cast<ChildWnd *>(subActivateWnd->widget());
        return subActivateCWnd;
    }
    else
        return nullptr;
}

void MainWindow::on_newAction_triggered()
{
    docNew();
}

void MainWindow::refreshMenus()//刷新窗口菜单
{
    bool hasActivateWnd;
    hasActivateWnd=(activateChildWnd()!=nullptr);
    ui->saveAction->setEnabled(hasActivateWnd);//保存
    ui->otherSaveAction->setEnabled(hasActivateWnd);//另存为
    ui->pasteAction->setEnabled(hasActivateWnd);//打印预览
    ui->printAction->setEnabled(hasActivateWnd);//打印
    ui->printViewAction->setEnabled(hasActivateWnd);//打印预览
    ui->nextAction->setEnabled(hasActivateWnd);//下一个窗口
    ui->previousAction->setEnabled(hasActivateWnd);//前一个窗口
    ui->titleAction->setEnabled(hasActivateWnd);//平铺窗口
    ui->casecadeAction->setEnabled(hasActivateWnd);//层叠窗口
    ui->closeAction->setEnabled(hasActivateWnd);//关闭窗口
    ui->closeAllAction->setEnabled(hasActivateWnd);//关闭所有子窗口

    bool hasSelect=(activateChildWnd()&&activateChildWnd()->textCursor().hasSelection());
    ui->cutAction->setEnabled(hasSelect);
    ui->copyAction->setEnabled(hasSelect);
    ui->boldAction->setEnabled(hasSelect);
    ui->italicAction->setEnabled(hasSelect);
    ui->leftAlignAction->setEnabled(hasSelect);
    ui->rightAlighAction->setEnabled(hasSelect);
    ui->underlineAction->setEnabled(hasSelect);
    ui->centerAction->setEnabled(hasSelect);
    ui->justifyAction->setEnabled(hasSelect);
    ui->colorAction->setEnabled(hasSelect);
}

void MainWindow::addSubWdnList()
{
    ui->menu_W->clear();//先将所有菜单列表项清空,再添加基础菜单列表
    ui->menu_W->addAction(ui->closeAction);
    ui->menu_W->addAction(ui->closeAllAction);
    ui->menu_W->addSeparator();
    ui->menu_W->addAction(ui->titleAction);
    ui->menu_W->addAction(ui->casecadeAction);
    ui->menu_W->addSeparator();
    ui->menu_W->addAction(ui->nextAction);
    ui->menu_W->addAction(ui->previousAction);
    ui->menu_W->addSeparator();

    QList<QMdiSubWindow*> Wnds=ui->mdiArea->subWindowList();//添加子窗口列表
    if(!Wnds.isEmpty())
        ui->menu_W->addSeparator();//为菜单列表添加分隔符

    for(int i=0;i<Wnds.size();++i){
        ChildWnd *childWnd=qobject_cast<ChildWnd*>(Wnds.at(i)->widget());//先将QMdiSubWindow*将QWidget*,再将QWidget*转换为ChildWnd*
        QString menu_text;
        menu_text=QString("%1%2").arg(i+1).arg(childWnd->getCurfileName());
        QAction* menuAction=ui->menu_W->addAction(menu_text);
        menuAction->setCheckable(true);
        menuAction->setChecked(childWnd==activateChildWnd());
        connect(menuAction,SIGNAL(triggered()),m_sigalMapper,SLOT(map()));//将菜单项点击信号与QSigalMapper的mapped方法建立关联
        m_sigalMapper->setMapping(menuAction,Wnds.at(i));//设置信号发送者的携带参数
    }
}

void MainWindow::on_closeAction_triggered()
{
    ui->mdiArea->closeActiveSubWindow();
}

void MainWindow::on_closeAllAction_triggered()
{
    ui->mdiArea->closeAllSubWindows();
}

void MainWindow::on_titleAction_triggered()
{
    ui->mdiArea->tileSubWindows();
}

void MainWindow::on_casecadeAction_triggered()
{
    ui->mdiArea->cascadeSubWindows();
}

void MainWindow::on_nextAction_triggered()
{
    ui->mdiArea->activateNextSubWindow();
}

void MainWindow::on_previousAction_triggered()
{
    ui->mdiArea->activatePreviousSubWindow();
}

void MainWindow::setSubActiveWindow(QWidget *widget)
{
    if(!widget)
        return ;
    ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(widget));
}

void MainWindow::on_openAction_triggered()
{
    docOpen();
}
