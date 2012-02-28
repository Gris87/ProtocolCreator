#include "src/other/global.h"

RegEditThread::RegEditThread(QObject *parent) :
    QThread(parent)
{
}

void RegEditThread::run()
{
#ifdef Q_OS_WIN
    // REGISTRATING EXTENSION
    QStringList arguments;
    QProcess aRegEdit;
    QProcess aRegEdit2;
    QProcess aRegEdit3;
    QProcess aRegEdit4;
    QProcess aRegEdit5;
    QProcess aRegEdit6;
    QProcess aRegEdit7;
    QProcess aRegEdit8;
    QProcess aRegEdit9;

    arguments.append("add");
    arguments.append("HKCR\\.pcr");
    arguments.append("/ve");
    arguments.append("/t");
    arguments.append("REG_SZ");
    arguments.append("/d");
    arguments.append("pcr_auto_file");
    arguments.append("/f");

    aRegEdit.start("reg",arguments);

    arguments.clear();
    arguments.append("add");
    arguments.append("HKCR\\Applications\\ProtocolCreator.exe\\shell\\open\\command");
    arguments.append("/ve");
    arguments.append("/t");
    arguments.append("REG_SZ");
    arguments.append("/d");
    arguments.append("\""+QDir::toNativeSeparators(QCoreApplication::applicationFilePath())+"\" \"%1\"");
    arguments.append("/f");

    aRegEdit2.start("reg",arguments);

    arguments.clear();
    arguments.append("add");
    arguments.append("HKCR\\pcr_auto_file\\shell\\open\\command");
    arguments.append("/ve");
    arguments.append("/t");
    arguments.append("REG_SZ");
    arguments.append("/d");
    arguments.append("\""+QDir::toNativeSeparators(QCoreApplication::applicationFilePath())+"\" \"%1\"");
    arguments.append("/f");

    aRegEdit3.start("reg",arguments);

    arguments.clear();
    arguments.append("add");
    arguments.append("HKCU\\Software\\Classes\\.pcr");
    arguments.append("/ve");
    arguments.append("/t");
    arguments.append("REG_SZ");
    arguments.append("/d");
    arguments.append("pcr_auto_file");
    arguments.append("/f");

    aRegEdit4.start("reg",arguments);

    arguments.clear();
    arguments.append("add");
    arguments.append("HKCU\\Software\\Classes\\Applications\\ProtocolCreator.exe\\shell\\open\\command");
    arguments.append("/ve");
    arguments.append("/t");
    arguments.append("REG_SZ");
    arguments.append("/d");
    arguments.append("\""+QDir::toNativeSeparators(QCoreApplication::applicationFilePath())+"\" \"%1\"");
    arguments.append("/f");

    aRegEdit5.start("reg",arguments);

    arguments.clear();
    arguments.append("add");
    arguments.append("HKCU\\Software\\Classes\\pcr_auto_file\\shell\\open\\command");
    arguments.append("/ve");
    arguments.append("/t");
    arguments.append("REG_SZ");
    arguments.append("/d");
    arguments.append("\""+QDir::toNativeSeparators(QCoreApplication::applicationFilePath())+"\" \"%1\"");
    arguments.append("/f");

    aRegEdit6.start("reg",arguments);

    arguments.clear();
    arguments.append("add");
    arguments.append("HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.pcr\\OpenWithList");
    arguments.append("/v");
    arguments.append("a");
    arguments.append("/t");
    arguments.append("REG_SZ");
    arguments.append("/d");
    arguments.append(QCoreApplication::applicationName());
    arguments.append("/f");

    aRegEdit7.start("reg",arguments);

    arguments.clear();
    arguments.append("add");
    arguments.append("HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.pcr\\OpenWithList");
    arguments.append("/v");
    arguments.append("MRUList");
    arguments.append("/t");
    arguments.append("REG_SZ");
    arguments.append("/d");
    arguments.append("a");
    arguments.append("/f");

    aRegEdit8.start("reg",arguments);

    arguments.clear();
    arguments.append("add");
    arguments.append("HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.pcr\\OpenWithProgids");
    arguments.append("/v");
    arguments.append("pcr_auto_file");
    arguments.append("/t");
    arguments.append("REG_NONE");
    arguments.append("/f");

    aRegEdit9.start("reg",arguments);

    aRegEdit.waitForFinished();
    aRegEdit2.waitForFinished();
    aRegEdit3.waitForFinished();
    aRegEdit4.waitForFinished();
    aRegEdit5.waitForFinished();
    aRegEdit6.waitForFinished();
    aRegEdit7.waitForFinished();
    aRegEdit8.waitForFinished();
    aRegEdit9.waitForFinished();
#endif
}
