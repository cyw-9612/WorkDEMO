#include "dialog.h"

#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QSharedMemory>
#include <QMessageBox>

void SetProcessAutoRunSelf(const QString &appPath)
{
    //注册表路径需要使用双反斜杠，如果是32位系统，要使用QSettings::Registry32Format
    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                       QSettings::Registry64Format);

    //以程序名称作为注册表中的键
    //根据键获取对应的值（程序路径）
    QFileInfo fInfo(appPath);
    QString name = fInfo.baseName();
    QString path = settings.value(name).toString();

    //如果注册表中的路径和当前程序路径不一样，
    //则表示没有设置自启动或自启动程序已经更换了路径
    //toNativeSeparators的意思是将"/"替换为"\"
    QString newPath = QDir::toNativeSeparators(appPath);
    if (path != newPath)
    {

        settings.setValue(name, newPath);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SetProcessAutoRunSelf(qApp->applicationFilePath());//创建程序自启注册表
    QSharedMemory mem("mypig");//以系统exe名称为参数，定义共享内存mem
    if(!mem.create(1))//创建共享内存mem，如果该共享内存已存在，则弹出提示对话框，并退出
    {
        QMessageBox::information(0,QObject::tr("Warning"),QObject::tr("程序已经运行！"));
        return 0;
    }

    Dialog w;
    w.show();
    return a.exec();
}
