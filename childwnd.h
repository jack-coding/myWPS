#ifndef CHILDWND_H
#define CHILDWND_H
#include <QTextEdit>

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
private slots:
    void docBeModified();//文档被修改时,给窗口标题栏加个*号

private:
    bool m_isSave;//文档是否保存
};

#endif // CHILDWND_H
