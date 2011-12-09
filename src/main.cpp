#include <QtGui/QApplication>
#include <QTextCodec>
#include "src/other/global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Q_INIT_RESOURCE(Resources);
    dir=QCoreApplication::applicationDirPath()+"/";

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));

    // INITIALIZATION
    mainWindow=new MainWindow();
    mainWindow->show();

    // RUNNING
    int res=a.exec();

    // FREE MEMORY
    delete mainWindow;

    return res;
}
