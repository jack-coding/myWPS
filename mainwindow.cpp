#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFont>
#include <QApplication>
#include "childwnd.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("myWPS");
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

void MainWindow::FormatSetEnable()
{
    ui->boldAction->setEnabled(true);
    ui->centerAction->setEnabled(true);
    ui->leftAlignAction->setEnabled(true);
    ui->rightAlighAction->setEnabled(true);
    ui->italicAction->setEnabled(true);
    ui->underlineAction->setEnabled(true);
    ui->justifyAction->setEnabled(true);
}

void MainWindow::on_newAction_triggered()
{
    docNew();
}
