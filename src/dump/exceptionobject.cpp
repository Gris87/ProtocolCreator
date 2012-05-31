#include "exceptionobject.h"

ExceptionObject::ExceptionObject(const QString &aReport, QObject *parent) :
    QObject(parent)
{
    mReport=aReport;
}

void ExceptionObject::showDialog()
{
    QString dir=QCoreApplication::applicationDirPath()+"/";

    QDir(dir).mkpath(dir+"crashes");
    QFile aFile(dir+"crashes/"+QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz")+".txt");

    aFile.open(QIODevice::WriteOnly);
    aFile.write(mReport.toUtf8());
    aFile.close();

    CrashDialog dialog(mReport);
    dialog.exec();

    QThread::currentThread()->quit();
}
