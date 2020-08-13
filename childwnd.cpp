#include "childwnd.h"
#include <QFileInfo>

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

void ChildWnd::docBeModified()
{
    setWindowModified(document()->isModified());
}
