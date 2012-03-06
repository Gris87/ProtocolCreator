#include <QtGui/QApplication>
#include <QTextCodec>
#include "src/other/global.h"

int main(int argc, char *argv[])
{
    qInstallMsgHandler(myMessageOutput);

    QApplication a(argc, argv);

    Q_INIT_RESOURCE(Resources);
    dir=QCoreApplication::applicationDirPath()+"/";

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    // INITIALIZATION
    functionsList.append("�����_�����_2");
    functionsList.append("��������_�����_2");
    functionsList.append("���������_2");
    functionsList.append("�������_2");
    functionsList.append("�������_2");
    functionsList.append("�����_������_2");
    functionsList.append("�����_������_2");
    functionsList.append("�����_�����_2");
    functionsList.append("�����_��_�����_2");
    functionsList.append("�����_������_�����_2");
    functionsList.append("�����_������_�����_2");
    functionsList.append("�����_�����_2");
    functionsList.append("������_�����_2");
    functionsList.append("������_��_�����_2");
    functionsList.append("������_�_2");
    functionsList.append("������_���_2");
    functionsList.append("������_��_1");
    functionsList.append("������_�����_2");
    functionsList.append("������_��_�����_2");
    functionsList.append("����_������_2");
    functionsList.append("����_������_2");
    functionsList.append("����_�����_2");
    functionsList.append("����_��_�����_2");
    functionsList.append("����_������_�����_2");
    functionsList.append("����_������_�����_2");
    functionsList.append("�����_������_2");
    functionsList.append("�����_������_2");
    functionsList.append("�����_�����_2");
    functionsList.append("�����_��_�����_2");
    functionsList.append("�����_������_�����_2");
    functionsList.append("�����_������_�����_2");
    functionsList.append("�����_�_������_1");
    functionsList.append("�����_�_������_1");
    functionsList.append("������_�_�����_1");
    functionsList.append("������_�_������_1");
    functionsList.append("������_�_�����_1");
    functionsList.append("������_�_������_1");
    functionsList.append("������_�_����_2");
    functionsList.append("������_��_�����_2");
    functionsList.append("������_�_������_2");
    functionsList.append("����_�_������_2");
    functionsList.append("�����_�_������_2");
    functionsList.append("������_�_������_2");
    functionsList.append("���_�����_1");
    functionsList.append("����_�����_2");
    functionsList.append("���������_2");
    functionsList.append("����������_2");
    functionsList.append("Sin_1");
    functionsList.append("Cos_1");
    functionsList.append("Tan_1");
    functionsList.append("CoTan_1");
    functionsList.append("��_0");
    functionsList.append("���������_3");
    functionsList.append("�����_������_1");
    functionsList.append("�������_�����_3");
    functionsList.append("��������_�����_3");
    functionsList.append("�������_�_������_2");
    functionsList.append("����_3");
    functionsList.append("�������_0");
    functionsList.append("����_1");
    functionsList.append("�����_1");
    functionsList.append("���_1");
    functionsList.append("������_0");
    functionsList.append("�������_1");
    functionsList.append("������_1");
    functionsList.append("���_1");
    functionsList.append("����_��������_������_2");
    functionsList.append("������������_�����_2");
    functionsList.append("�������_������_3");
    functionsList.append("��������_��_��_������_3");
    functionsList.append("��������_���_���������_4");
    functionsList.append("��������_����_������_1");

    mainWindow=new MainWindow();
    globalDialog=new GlobalDialog(mainWindow);
    mainWindow->show();

    if (argc>1)
    {
        QStringList arguments=QApplication::arguments();

        mainWindow->openFile(arguments.last());
    }
    else
    {
        if (mainWindow->mLastAutoSave=="")
        {
            mainWindow->on_actionNew_triggered();
        }
        else
        {
            mainWindow->openFile(mainWindow->mLastAutoSave);
            QMessageBox::warning(mainWindow, protocolCreatorVersion, "ProtocolCreator ������������ ����� ����");
        }
    }

    mainWindow->ui->progressBar->repaint();

    RegEditThread aRegEditThread;
    aRegEditThread.start(QThread::IdlePriority);

    // RUNNING
    int res=a.exec();

    // FREE MEMORY
    delete mainWindow;

    return res;
}
