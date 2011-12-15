#include "src/other/global.h"

void copyVariable(PageComponent *aComponent)
{
    PageSelectionDialog dialog(mainWindow);

    dialog.ui->titleLabel->setText("Выберите раздел, в который будет скопирована переменная:");

    if (dialog.exec())
    {
        int aRow=dialog.ui->pagesListWidget->currentRow();

        if (aRow>0 && mainWindow->ui->pagesTabWidget->widget(aRow-1)==mainWindow->contentPage)
        {
            QMessageBox::information(mainWindow, protocolCreatorVersion, "Нельзя вставлять переменную в содержание");
            return;
        }

        QByteArray aBuffer;
        QDataStream aStream(&aBuffer, QIODevice::ReadWrite);

        aComponent->saveToStream(aStream);

        aStream.device()->seek(0);

        QString aMagicWord;
        aStream >> aMagicWord;

        PageComponent *aVariable=0;

        if (aMagicWord=="VarInteger")
        {
            aVariable=new VariableIntegerFrame(mainWindow);
        }
        else
        if (aMagicWord=="VarString")
        {
            aVariable=new VariableStringFrame(mainWindow);
        }
        else
        if (aMagicWord=="VarBoolean")
        {
            aVariable=new VariableBoolFrame(mainWindow);
        }
        else
        if (aMagicWord=="VarDate")
        {
            aVariable=new VariableDateFrame(mainWindow);
        }
        else
        if (aMagicWord=="VarTime")
        {
            aVariable=new VariableTimeFrame(mainWindow);
        }
        else
        if (aMagicWord=="VarList")
        {
            aVariable=new VariableListFrame(mainWindow);
        }
        else
        if (aMagicWord=="VarExtendedList")
        {
            aVariable=new VariableExtendedListFrame(this);

            ((VariableExtendedListFrame*)aVariable)->ui->useCheckBox->setVisible(false);
        }

        if (aVariable)
        {
            if (aRow==0)
            {
                globalDialog->addVariable(aVariable);
            }
            else
            {
                ((PageFrame*)mainWindow->ui->pagesTabWidget->widget(aRow-1))->addVariable(aVariable);
            }

            aVariable->loadFromStream(aStream);
        }
    }
}

void checkVarName(QString aName, QStringList &aErrorList)
{
    for (int i=0; i<aName.length(); i++)
    {
        if (
            !(aName.at(i)>='a' && aName.at(i)<='z')
            &&
            !(aName.at(i)>='A' && aName.at(i)<='Z')
            &&
            !(aName.at(i)>='0' && aName.at(i)<='9')
            &&
            aName.at(i)!='_'
           )
        {
            aErrorList.append("Error: Недопустимый символ \""+QString(aName.at(i))+"\" в имени");
        }
    }

    if (aName.length()>0 && aName.at(0)>='0' && aName.at(0)<='9')
    {
        aErrorList.append("Error: Имя не может начинаться с цифры");
    }
}
