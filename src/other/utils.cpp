#include "src/other/global.h"

void copyVariable(PageComponent *aComponent)
{
    PageSelectionDialog dialog(mainWindow);

    dialog.ui->titleLabel->setText("¬ыберите раздел, в который будет скопирована переменна€:");

    if (dialog.exec())
    {
        int aRow=dialog.ui->pagesListWidget->currentRow();

        if (aRow>0 && mainWindow->ui->pagesTabWidget->widget(aRow-1)==mainWindow->contentPage)
        {
            QMessageBox::information(mainWindow, protocolCreatorVersion, "Ќельз€ вставл€ть переменную в содержание");
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
