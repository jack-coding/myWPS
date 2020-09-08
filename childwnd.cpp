#include "childwnd.h"
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QMessageBox>
#include <QTextFormat>
#include <QTextListFormat>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTextListFormat>
#include <QtWidgets>

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

bool ChildWnd::saveDoc()
{
    if(this->m_isSave)
        return saveDocOpt(m_curfilePath);
    else
        return saveAsDoc();//如果没有保存,那就另存为
}

bool ChildWnd::saveAsDoc()
{
    QString docName=QFileDialog::getSaveFileName(this,"另存为",m_curfilePath,"HTML文档(*.htm *html);;所有文件(*.*)");
    if(docName.isEmpty()) //如果文件名为空,直接返回false
        return false;
    else//文件名不为空,保存文件
        return saveDocOpt(docName);
}

bool ChildWnd::saveDocOpt( QString &filename)
{
    //如果文件不是以htm或者html结尾的话,添加文件后缀.html
    if(!(filename.endsWith(".htm",Qt::CaseInsensitive)||filename.endsWith(".html",Qt::CaseInsensitive))){
        filename+=".html";
    }

   QTextDocumentWriter writer(filename);//用QTextDocumentWriter类对象的write方法实现文件的保存;
   bool isSuccess=writer.write(this->document());
   if(isSuccess) setCurDoc(filename);//如果保存成功,将文件的文件名重置
   return isSuccess;//返回文件的保存结果
}

void ChildWnd::setFontFormatOnSelect(const QTextCharFormat &format)
{
    QTextCursor tcursor=textCursor();//获取当前光标
    if(!tcursor.hasSelection()){
        tcursor.select(QTextCursor::WordUnderCursor);//如果光标没有选中,那就选中光标选中的字符串
    }
    //合并文本格式
    tcursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}

void ChildWnd::setAlignOfDocumentText(int aligntype)
{
    if(aligntype==1)//左对齐
        setAlignment(Qt::AlignLeft|Qt::AlignAbsolute);
    else if(aligntype==2){//右对齐
        setAlignment(Qt::AlignRight|Qt::AlignAbsolute);
    }
    else if(aligntype==3){//居中对齐
        setAlignment(Qt::AlignCenter);
    }
    else if(aligntype==4){//两端对齐
        setAlignment(Qt::AlignJustify);
    }
}

void ChildWnd::setParaStyle(int pstyle)
{
    QTextCursor tcursor=textCursor();//获取光标

    QTextListFormat::Style sname;
    if(pstyle!=0)
    {
        switch(pstyle){
        case 1:
            sname=QTextListFormat::ListDisc;//实心圆
        break;
        case 2:
            sname=QTextListFormat::ListCircle;//空心圆
        break;
        case 3:
            sname=QTextListFormat::ListSquare;//实心方形
        break;
        case 4:
            sname=QTextListFormat::ListDecimal;//十进制整数
        break;
        case 5:
            sname=QTextListFormat::ListLowerAlpha;//小写字母
        break;
        case 6:
            sname=QTextListFormat::ListUpperAlpha;//大写字母
        break;
        case 7:
            sname=QTextListFormat::ListLowerRoman;//小写罗马
        break;
        case 8:
            sname=QTextListFormat::ListUpperRoman;//大写罗马
        break;
        default:
            sname=QTextListFormat::ListDisc;//实心圆
        break;
        }

        tcursor.beginEditBlock();

        QTextBlockFormat tBlockFmt=tcursor.blockFormat();
        QTextListFormat tListFmt;
        if(tcursor.currentList()){//如果当前光标选中区域有项目列表,将列表格式设置为当前的列表格式
            tListFmt=tcursor.currentList()->format();
        }
        else{//如果没有项目列表,设置项目列表的格式
            tListFmt.setIndent(tBlockFmt.indent()+1);//1表示一个tab键的宽度
            tBlockFmt.setIndent(0);
            tcursor.setBlockFormat(tBlockFmt);
        }
        tListFmt.setStyle(sname);//设置列表格式
        tcursor.createList(tListFmt);//将这个列表格式对象作为光标选中文本的格式

        tcursor.endEditBlock();
    }
    else{
        QTextBlockFormat tbfmt;//如果传入数字等于零,则将格式设置为无效
        tbfmt.setObjectIndex(-1);
        tcursor.mergeBlockFormat(tbfmt);
    }
}


void ChildWnd::docBeModified()
{
    setWindowModified(document()->isModified());
}

void ChildWnd::closeEvent(QCloseEvent *event)
{
    if(promptSave())//如果提示保存成功(用户保存了文件),则接受关闭事件
        event->accept();
    else//如果保存文件失败,那么忽略关闭事件
        event->ignore();
}

bool ChildWnd::promptSave()
{
    if(!document()->isModified())//如果没有被修改,返回true;
        return true;

    QMessageBox::StandardButton result;
    result=QMessageBox::warning(this,"系统提示",QString("文档%1已修改,是否保存?").arg(getCurfileName()),
                                QMessageBox::Yes|QMessageBox::Discard|QMessageBox::No);
    if(result==QMessageBox::Yes)
            return saveDoc();
    else if(result==QMessageBox::No)
        return false;
    return true;
}
