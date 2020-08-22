#include "childwnd.h"
#include <QFileInfo>
#include<QFile>

ChildWnd::ChildWnd()
{
    setAttribute(Qt::WA_DeleteOnClose);//子窗口关闭时,销毁该类的实例对象
    m_isSave=false;//暂未保存
}

void ChildWnd::newDoc()
{
    static int number=1;//文档编号
    m_curfilePath=QString("myWPS 文档 %1").arg(number++);
    //修改窗口标题栏
    setWindowTitle(m_curfilePath+"[*]");
    connect(document(),SIGNAL(contentsChanged()),this,SLOT(docBeModified()));
}

QString ChildWnd::getCurfileName()
{
    return QFileInfo(m_curfilePath).fileName();
}

bool ChildWnd::loadFile(const QString &docName)
{
    if(docName.isEmpty())
        return false;
    QFile file(docName);
    if(!file.exists())
        return false;
    if(!file.open(QFile::ReadOnly))
        return false;
    QByteArray text=file.readAll();
    if(Qt::mightBeRichText(text))
        setHtml(text);//如果是富文本,则将文件格式设置为html格式
    else
        setPlainText(text);//如果是纯文本,则将文件格式设置为纯文本格式
    setCurDoc(docName);//设置活动窗口标题
    connect(document(),SIGNAL(contentsChanged()),this,SLOT(docBeModified()));//一旦文件修改,将会修改活动子窗口的标题
    return true;
}

void ChildWnd::setCurDoc(const QString &docName)
{
    //canonicalFilePath()返回标准名称路径,可以过滤文件名中的"." ".."
    m_curfilePath=QFileInfo(docName).canonicalFilePath();//获取文件的绝对路径
    m_isSave=true;//文件保存状态设置为已保存
    document()->setModified(false);//文件暂未被修改
    setWindowModified(false);//文件暂未被修改
    setWindowTitle(docName+"[*]");//设置活动窗口标题
}

void ChildWnd::docBeModified()
{
    setWindowModified(document()->isModified());
}
