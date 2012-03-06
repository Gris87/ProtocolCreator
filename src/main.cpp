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
    functionsList.append("Сумма_чисел_2");
    functionsList.append("Разность_чисел_2");
    functionsList.append("Умножение_2");
    functionsList.append("Деление_2");
    functionsList.append("Степень_2");
    functionsList.append("Число_Больше_2");
    functionsList.append("Число_Меньше_2");
    functionsList.append("Число_Равно_2");
    functionsList.append("Число_Не_Равно_2");
    functionsList.append("Число_Больше_Равно_2");
    functionsList.append("Число_Меньше_Равно_2");
    functionsList.append("Сумма_строк_2");
    functionsList.append("Строки_Равны_2");
    functionsList.append("Строки_Не_Равны_2");
    functionsList.append("Логика_И_2");
    functionsList.append("Логика_ИЛИ_2");
    functionsList.append("Логика_НЕ_1");
    functionsList.append("Логика_Равна_2");
    functionsList.append("Логика_Не_Равна_2");
    functionsList.append("Дата_Больше_2");
    functionsList.append("Дата_Меньше_2");
    functionsList.append("Дата_Равна_2");
    functionsList.append("Дата_Не_Равна_2");
    functionsList.append("Дата_Больше_Равна_2");
    functionsList.append("Дата_Меньше_Равна_2");
    functionsList.append("Время_Больше_2");
    functionsList.append("Время_Меньше_2");
    functionsList.append("Время_Равно_2");
    functionsList.append("Время_Не_Равно_2");
    functionsList.append("Время_Больше_Равно_2");
    functionsList.append("Время_Меньше_Равно_2");
    functionsList.append("Число_в_Логику_1");
    functionsList.append("Число_в_Строку_1");
    functionsList.append("Логика_в_Число_1");
    functionsList.append("Логика_в_строку_1");
    functionsList.append("Строка_в_Число_1");
    functionsList.append("Строка_в_Логику_1");
    functionsList.append("Строка_в_Дату_2");
    functionsList.append("Строка_во_Время_2");
    functionsList.append("Строка_в_Список_2");
    functionsList.append("Дата_в_Строку_2");
    functionsList.append("Время_в_Строку_2");
    functionsList.append("Список_в_Строку_2");
    functionsList.append("Это_число_1");
    functionsList.append("Нули_слева_2");
    functionsList.append("Случайный_2");
    functionsList.append("Округление_2");
    functionsList.append("Sin_1");
    functionsList.append("Cos_1");
    functionsList.append("Tan_1");
    functionsList.append("CoTan_1");
    functionsList.append("ПИ_0");
    functionsList.append("Подстрока_3");
    functionsList.append("Длина_текста_1");
    functionsList.append("Удалить_текст_3");
    functionsList.append("Вставить_текст_3");
    functionsList.append("Позиция_в_тексте_2");
    functionsList.append("Если_3");
    functionsList.append("Сегодня_0");
    functionsList.append("День_1");
    functionsList.append("Месяц_1");
    functionsList.append("Год_1");
    functionsList.append("Сейчас_0");
    functionsList.append("Секунда_1");
    functionsList.append("Минута_1");
    functionsList.append("Час_1");
    functionsList.append("Срок_Годности_Пришел_2");
    functionsList.append("Используемые_ГОСТы_2");
    functionsList.append("Найдена_строка_3");
    functionsList.append("Значение_не_из_списка_3");
    functionsList.append("Значения_вне_диапазона_4");
    functionsList.append("Возможно_Пром_Строка_1");

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
            QMessageBox::warning(mainWindow, protocolCreatorVersion, "ProtocolCreator восстановлен после сбоя");
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
