#ifndef CHILDWND_H
#define CHILDWND_H
#include <QTextEdit>
#include<QCloseEvent>
#include<QTextCharFormat>

class ChildWnd : public QTextEdit
{
    Q_OBJECT
public:
    ChildWnd();
    QString m_curfilePath;//保存文档的路径
    void newDoc();//新建文档
    QString getCurfileName();//从文档路径中提取文档名
    bool loadFile(const QString& docName);
    void setCurDoc(const QString& docName);
    bool saveDoc();//保存文件
    bool saveAsDoc();//另存文件
    bool saveDocOpt(QString &filename);
    void setFontFormatOnSelect(const QTextCharFormat& format);//设置选中字体的格式
    void setAlignOfDocumentText(int aligntype);//设置文字的位置(居左 居中 居右 两端对齐)
    void setParaStyle(int pstyle);
private slots:
    void docBeModified();//文档被修改时,给窗口标题栏加个*号
protected:
    void closeEvent(QCloseEvent *event);//重写关闭事件
private:
    bool promptSave();
    bool m_isSave;//文档是否保存
};

#endif // CHILDWND_H
