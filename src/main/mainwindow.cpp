#include "src/other/global.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    dividerSplitter = new QSplitter(Qt::Vertical, this);
    ui->dividerLayout->removeWidget(ui->pagesTabWidget);
    ui->dividerLayout->removeWidget(ui->logListWidget);

    dividerSplitter->addWidget(ui->pagesTabWidget);
    dividerSplitter->addWidget(ui->logListWidget);

    ui->dividerLayout->addWidget(dividerSplitter);

    connect(ui->pagesTabWidget->tabBar(), SIGNAL(tabMoved(int,int)), this, SLOT(pageMoved(int,int)));

    loadState();

    on_actionNew_triggered();

    ui->progressBar->setMaximum(1);
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    saveState();

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int aAnswer=QMessageBox::question(this, protocolCreatorVersion, "Вы хотите сохранить работу перед выходом?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No, QMessageBox::Cancel | QMessageBox::Escape);

    if (aAnswer==QMessageBox::Yes)
    {
        on_actionSave_triggered();
        event->accept();
    }
    else
    if (aAnswer==QMessageBox::No)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::on_actionNew_triggered()
{
    currentName="";
    isAdmin=true;
    docPass="";
    adminPass="";

    updateHeader();

    if (globalDialog)
    {
        while (globalDialog->variables.length()>0)
        {
            QWidget* aWidget=globalDialog->variables.takeFirst();
            globalDialog->ui->variableLayout->removeWidget(aWidget);
            delete aWidget;
        }
    }

    ui->progressBar->setMaximum(ui->pagesTabWidget->count());

    while (ui->pagesTabWidget->count()>0)
    {
        ui->progressBar->setValue(ui->progressBar->maximum()-ui->pagesTabWidget->count());

        QWidget *aWidget=ui->pagesTabWidget->widget(0);
        ui->pagesTabWidget->removeTab(0);
        delete aWidget;
    }

    ui->progressBar->setValue(0);

    contentPage=0;
    addPage("Содержание", "Section_Content");

    updateAdmin();
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog(this, protocolCreatorVersion, currentName, "*.pcr");

    if (dialog.exec())
    {
        QByteArray aArray;

        QFile aFile(dialog.selectedFiles().at(0));
        aFile.open(QIODevice::ReadOnly);
        aArray=aFile.readAll();
        aFile.close();

        DecryptStream(aArray, "Thunderbolt");

        QDataStream aStream(&aArray, QIODevice::ReadOnly);

        QString aMagicWord;
        aStream >> aMagicWord;

        if (aMagicWord!="ProtocolCreator")
        {
            QMessageBox::warning(this, "Открытие файла", "Неверный формат");
            return;
        }

        aStream >> aMagicWord;

        if (aMagicWord!="DocumentPassword")
        {
            QMessageBox::warning(this, "Открытие файла", "Неверный формат");
            return;
        }

        QString oldDocPass=docPass;

        aStream >> docPass;

        if (docPass.length()>2)
        {
            PasswordDialog dialog(this);
            dialog.ui->titleLabel->setText("Введите пароль на документ");

            if (dialog.exec())
            {
                if (EncryptString(dialog.ui->passwordEdit->text(), "Earthquake")!=docPass)
                {
                    QMessageBox::information(this, "Ввод пароля", "Неверный пароль");

                    docPass=oldDocPass;

                    return;
                }
            }
        }
        else
        {
            docPass="";
        }

        currentName=dialog.selectedFiles().at(0);
        isAdmin=false;
        adminPass="";

        while (globalDialog->variables.length()>0)
        {
            QWidget* aWidget=globalDialog->variables.takeFirst();
            globalDialog->ui->variableLayout->removeWidget(aWidget);
            delete aWidget;
        }

        ui->progressBar->setMaximum(ui->pagesTabWidget->count());

        while (ui->pagesTabWidget->count()>0)
        {
            ui->progressBar->setValue(ui->progressBar->maximum()-ui->pagesTabWidget->count());

            QWidget *aWidget=ui->pagesTabWidget->widget(0);
            ui->pagesTabWidget->removeTab(0);
            delete aWidget;
        }

        ui->progressBar->setValue(0);

        ui->progressBar->setMaximum(aArray.length());

        contentPage=0;

        while (!aStream.atEnd())
        {
            ui->progressBar->setValue(aStream.device()->pos());

            aStream >> aMagicWord;

            if (aMagicWord=="AdminPassword")
            {
                aStream >> adminPass;
            }
            else
            if (aMagicWord=="Global")
            {
                globalDialog->loadFromStream(aStream);
            }
            else
            if (aMagicWord=="Pages")
            {
                while (!aStream.atEnd())
                {
                    ui->progressBar->setValue(aStream.device()->pos());

                    aStream >> aMagicWord;

                    if (aMagicWord=="Stop")
                    {
                        break;
                    }

                    addPage("", "");

                    ((PageFrame*)ui->pagesTabWidget->widget(ui->pagesTabWidget->count()-1))->loadFromStream(aStream);
                }
            }
            else
            if (aMagicWord=="ContentIndex")
            {
                int pageIndex;

                aStream >> pageIndex;

                if (pageIndex>0)
                {
                    ui->pagesTabWidget->tabBar()->moveTab(0, pageIndex);
                    ui->pagesTabWidget->setCurrentIndex(0);
                    ui->pagesTabWidget->setCurrentIndex(pageIndex);
                }
            }
        }

        ui->progressBar->setValue(0);

        updateHeader();
        updateAdmin();
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (currentName=="")
    {
        on_actionSaveAs_triggered();
    }
    else
    {
        QByteArray aArray;
        QDataStream aStream(&aArray, QIODevice::WriteOnly);

        aStream << QString("ProtocolCreator");

        aStream << QString("DocumentPassword");
        aStream << docPass;

        aStream << QString("AdminPassword");
        aStream << adminPass;

        globalDialog->saveToStream(aStream);

        aStream << QString("Pages");

        contentPage->saveToStream(aStream);

        int pageIndex=ui->pagesTabWidget->indexOf(contentPage);

        ui->progressBar->setMaximum(ui->pagesTabWidget->count());

        for (int i=0; i<ui->pagesTabWidget->count(); i++)
        {
            ui->progressBar->setValue(i);

            if (i!=pageIndex)
            {
                ((PageFrame*)ui->pagesTabWidget->widget(i))->saveToStream(aStream);
            }
        }

        ui->progressBar->setValue(0);

        aStream << QString("Stop");

        aStream << QString("ContentIndex");
        aStream << pageIndex;

        EncryptStream(aArray, "Thunderbolt");

        // Save result
        QFile aFile(currentName);

        // Backup existing file
        if (aFile.exists())
        {
            QDir(dir).mkpath(dir+"backup");

            QString baseName=currentName.mid(currentName.lastIndexOf("/")+1);
            baseName=baseName.left(baseName.lastIndexOf("."));

            QString backupName=dir+"backup/"+baseName+QFileInfo(aFile).lastModified().toString("yyyy_MM_dd_hh_mm_ss_zzz")+".pcr";

            if (QFile::exists(backupName))
            {
                QFile::remove(backupName);
            }

            aFile.copy(backupName);
        }

        aFile.open(QIODevice::WriteOnly);
        aFile.write(aArray);
        aFile.close();
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QFileDialog dialog(this, protocolCreatorVersion, currentName, "*.pcr");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("pcr");

    if (dialog.exec())
    {
        currentName=dialog.selectedFiles().at(0);
        on_actionSave_triggered();

        updateHeader();
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionCheckDocument_triggered()
{
    errorHappened=false;

    ui->logListWidget->clear();

    ui->progressBar->setMaximum(9);
    ui->progressBar->setValue(0);

    for (int i=0; i<globalDialog->variables.length(); i++)
    {
        if (globalDialog->variables.at(i)->name().trimmed()=="")
        {
            addHint("Нет заголовка у глобальной переменной");
        }

        if (globalDialog->variables.at(i)->variableName().trimmed()=="")
        {
            addError("Нет имени у глобальной переменной \""+globalDialog->variables.at(i)->name()+"\"");
        }
    }

    ui->progressBar->setValue(ui->progressBar->value()+1);

    for (int i=0; i<globalDialog->variables.length()-1; i++)
    {
        for (int j=i+1; j<globalDialog->variables.length(); j++)
        {
            if (globalDialog->variables.at(i)->variableName()==globalDialog->variables.at(j)->variableName())
            {
                addError("Одинаковые имена у глобальной переменной \""+globalDialog->variables.at(i)->name()+"\" и у глобальной переменной \""+globalDialog->variables.at(j)->name()+"\"");
            }
        }
    }

    ui->progressBar->setValue(ui->progressBar->value()+1);

    for (int i=0; i<ui->pagesTabWidget->count(); i++)
    {
        PageFrame* aPage=((PageFrame*)ui->pagesTabWidget->widget(i));

        if (aPage->ui->nameEdit->text().trimmed()=="")
        {
            addHint("Нет заголовка у раздела");
        }

        if (aPage->ui->varNameEdit->text().trimmed()=="")
        {
            addError("Нет имени у раздела \""+aPage->ui->nameEdit->text()+"\"");
        }
        else
        if (aPage->ui->varNameEdit->text().trimmed()=="Global")
        {
            addError("Имя раздела \""+aPage->ui->nameEdit->text()+"\" совпадает с Global");
        }
        else
        {
            QStringList aErrors;

            checkVarName(aPage->ui->varNameEdit->text(), aErrors);

            for (int j=0; j<aErrors.length(); j++)
            {
                addLog("Раздел \""+aPage->ui->varNameEdit->text()+"\"", aErrors.at(j));
            }
        }
    }

    ui->progressBar->setValue(ui->progressBar->value()+1);

    for (int i=0; i<ui->pagesTabWidget->count()-1; i++)
    {
        PageFrame* aPage=((PageFrame*)ui->pagesTabWidget->widget(i));

        for (int j=i+1; j<ui->pagesTabWidget->count(); j++)
        {
            PageFrame* aPage2=((PageFrame*)ui->pagesTabWidget->widget(j));

            if (aPage->ui->varNameEdit->text()==aPage2->ui->varNameEdit->text())
            {
                addError("Одинаковые имена у раздела \""+aPage->ui->nameEdit->text()+"\" и у раздела \""+aPage2->ui->nameEdit->text()+"\"");
            }
        }
    }

    ui->progressBar->setValue(ui->progressBar->value()+1);

    for (int i=0; i<ui->pagesTabWidget->count(); i++)
    {
        PageFrame* aPage=((PageFrame*)ui->pagesTabWidget->widget(i));

        for (int j=0; j<globalDialog->variables.length(); j++)
        {
            if (globalDialog->variables.at(j)->variableName()==aPage->ui->varNameEdit->text())
            {
                addError("Одинаковые имена у глобальной переменной \""+globalDialog->variables.at(j)->name()+"\" и у раздела \""+aPage->ui->nameEdit->text()+"\"");
            }
        }
    }

    ui->progressBar->setValue(ui->progressBar->value()+1);

    for (int i=0; i<ui->pagesTabWidget->count(); i++)
    {
        PageFrame* aPage=((PageFrame*)ui->pagesTabWidget->widget(i));

        for (int j=0; j<aPage->variables.length(); j++)
        {
            if (aPage->variables.at(j)->name()=="")
            {
                addHint("Нет заголовка у переменной в разделе \""+aPage->ui->nameEdit->text()+"\"");
            }

            if (aPage->variables.at(j)->variableName()=="")
            {
                addError("Нет имени у переменной \""+aPage->variables.at(j)->name()+"\" в разделе \""+aPage->ui->nameEdit->text()+"\"");
            }
        }

        for (int j=0; j<aPage->components.length(); j++)
        {
            if (aPage->components.at(j)->name()=="")
            {
                addHint("Нет заголовка у компонента в разделе \""+aPage->ui->nameEdit->text()+"\"");
            }

            if (aPage->components.at(j)->variableName()=="")
            {
                addError("Нет имени у компонента \""+aPage->components.at(j)->name()+"\" в разделе \""+aPage->ui->nameEdit->text()+"\"");
            }
        }
    }

    ui->progressBar->setValue(ui->progressBar->value()+1);

    for (int i=0; i<ui->pagesTabWidget->count(); i++)
    {
        PageFrame* aPage=((PageFrame*)ui->pagesTabWidget->widget(i));

        for (int j=0; j<aPage->variables.length(); j++)
        {
            for (int k=j+1; k<aPage->variables.length(); k++)
            {
                if (aPage->variables.at(j)->variableName()==aPage->variables.at(k)->variableName())
                {
                    addError("Одинаковые имена у переменной \""+aPage->variables.at(j)->name()+"\" и у переменной \""+aPage->variables.at(k)->name()+"\" в разделе \""+aPage->ui->nameEdit->text()+"\"");
                }
            }
        }

        for (int j=0; j<aPage->components.length(); j++)
        {
            for (int k=j+1; k<aPage->components.length(); k++)
            {
                if (aPage->components.at(j)->variableName()==aPage->components.at(k)->variableName())
                {
                    addError("Одинаковые имена у компоненты \""+aPage->components.at(j)->name()+"\" и у компоненты \""+aPage->components.at(k)->name()+"\" в разделе \""+aPage->ui->nameEdit->text()+"\"");
                }
            }
        }

        for (int j=0; j<aPage->variables.length(); j++)
        {
            for (int k=0; k<aPage->components.length(); k++)
            {
                if (aPage->variables.at(j)->variableName()==aPage->components.at(k)->variableName())
                {
                    addError("Одинаковые имена у переменной \""+aPage->variables.at(j)->name()+"\" и у компоненты \""+aPage->components.at(k)->name()+"\" в разделе \""+aPage->ui->nameEdit->text()+"\"");
                }
            }
        }
    }

    ui->progressBar->setValue(ui->progressBar->value()+1);

    for (int i=0; i<ui->pagesTabWidget->count(); i++)
    {
        PageFrame* aPage=((PageFrame*)ui->pagesTabWidget->widget(i));

        for (int j=0; j<aPage->variables.length(); j++)
        {
            for (int k=0; k<globalDialog->variables.length(); k++)
            {
                if (aPage->variables.at(j)->variableName()==globalDialog->variables.at(k)->variableName())
                {
                    addError("Одинаковые имена у глобальной переменной \""+globalDialog->variables.at(k)->name()+"\" и у переменной \""+aPage->variables.at(j)->name()+"\" в разделе \""+aPage->ui->nameEdit->text()+"\"");
                }
            }
        }

        for (int j=0; j<aPage->components.length(); j++)
        {
            for (int k=0; k<globalDialog->variables.length(); k++)
            {
                if (aPage->components.at(j)->variableName()==globalDialog->variables.at(k)->variableName())
                {
                    addError("Одинаковые имена у глобальной переменной \""+globalDialog->variables.at(k)->name()+"\" и у компоненты \""+aPage->components.at(j)->name()+"\" в разделе \""+aPage->ui->nameEdit->text()+"\"");
                }
            }
        }
    }

    ui->progressBar->setValue(ui->progressBar->value()+1);

    for (int i=0; i<ui->pagesTabWidget->count(); i++)
    {
        PageFrame* aPage=((PageFrame*)ui->pagesTabWidget->widget(i));

        for (int j=0; j<ui->pagesTabWidget->count(); j++)
        {
            PageFrame* aPage2=((PageFrame*)ui->pagesTabWidget->widget(j));

            for (int k=0; k<aPage->variables.length(); k++)
            {
                if (aPage->variables.at(k)->variableName()==aPage2->ui->varNameEdit->text())
                {
                    addError("Одинаковые имена у переменной \""+aPage->variables.at(k)->name()+"\" в разделе \""+aPage->ui->nameEdit->text()+"\" и у раздела \""+aPage2->ui->nameEdit->text()+"\"");
                }
            }

            for (int k=0; k<aPage->components.length(); k++)
            {
                if (aPage->components.at(k)->variableName()==aPage2->ui->varNameEdit->text())
                {
                    addError("Одинаковые имена у компоненты \""+aPage->components.at(k)->name()+"\" в разделе \""+aPage->ui->nameEdit->text()+"\" и у раздела \""+aPage2->ui->nameEdit->text()+"\"");
                }
            }
        }
    }

    ui->progressBar->setValue(ui->progressBar->value()+1);

    int progressMax=globalDialog->variables.length();

    for (int i=0; i<ui->pagesTabWidget->count(); i++)
    {
        PageFrame* aPage=((PageFrame*)ui->pagesTabWidget->widget(i));

        progressMax+=aPage->variables.length();
        progressMax+=aPage->components.length();
    }

    if (progressMax>0)
    {
        ui->progressBar->setMaximum(progressMax);
        ui->progressBar->setValue(0);

        QStringList aErrors;

        for (int i=0; i<globalDialog->variables.length(); i++)
        {
            globalDialog->variables.at(i)->resetCalculation();
            globalDialog->variables.at(i)->checkForErrors(aErrors);

            for (int j=0; j<aErrors.length(); j++)
            {
                addLog("Global."+globalDialog->variables.at(i)->variableName(), aErrors.at(j));
            }

            aErrors.clear();

            ui->progressBar->setValue(ui->progressBar->value()+1);
        }

        for (int i=0; i<ui->pagesTabWidget->count(); i++)
        {
            PageFrame* aPage=((PageFrame*)ui->pagesTabWidget->widget(i));

            for (int j=0; j<aPage->variables.length(); j++)
            {
                aPage->variables.at(j)->resetCalculation();
                aPage->variables.at(j)->checkForErrors(aErrors);

                for (int k=0; k<aErrors.length(); k++)
                {
                    addLog(aPage->ui->varNameEdit->text()+"."+aPage->variables.at(j)->variableName(), aErrors.at(k));
                }

                aErrors.clear();

                ui->progressBar->setValue(ui->progressBar->value()+1);
            }

            for (int j=0; j<aPage->components.length(); j++)
            {
                aPage->components.at(j)->resetCalculation();
                aPage->components.at(j)->checkForErrors(aErrors);

                for (int k=0; k<aErrors.length(); k++)
                {
                    addLog(aPage->ui->varNameEdit->text()+"."+aPage->components.at(j)->variableName(), aErrors.at(k));
                }

                aErrors.clear();

                ui->progressBar->setValue(ui->progressBar->value()+1);
            }
        }

        ui->progressBar->setValue(0);

        for (int i=0; i<globalDialog->variables.length(); i++)
        {
            try
            {
                globalDialog->variables.at(i)->calculate();
            }
            catch(...)
            {
                addError("Global."+globalDialog->variables.at(i)->variableName()+": "+globalDialog->variables.at(i)->calculationError);
            }

            ui->progressBar->setValue(ui->progressBar->value()+1);
        }

        for (int i=0; i<ui->pagesTabWidget->count(); i++)
        {
            PageFrame* aPage=((PageFrame*)ui->pagesTabWidget->widget(i));

            for (int j=0; j<aPage->variables.length(); j++)
            {
                try
                {
                    aPage->variables.at(j)->calculate();
                }
                catch(...)
                {
                    addError(aPage->ui->varNameEdit->text()+"."+aPage->variables.at(j)->variableName()+": "+aPage->variables.at(j)->calculationError);
                }

                ui->progressBar->setValue(ui->progressBar->value()+1);
            }

            for (int j=0; j<aPage->components.length(); j++)
            {
                try
                {
                    aPage->components.at(j)->calculate();
                }
                catch(...)
                {
                    addError(aPage->ui->varNameEdit->text()+"."+aPage->components.at(j)->variableName()+": "+aPage->components.at(j)->calculationError);
                }

                ui->progressBar->setValue(ui->progressBar->value()+1);
            }
        }
    }

    ui->progressBar->setValue(0);

    if (ui->logListWidget->count()>0)
    {
        if (ui->logListWidget->height()==0)
        {
            QList<int> aSizes;

            aSizes.append(ui->pagesTabWidget->height()*0.9);
            aSizes.append(ui->pagesTabWidget->height()*0.1);

            dividerSplitter->setSizes(aSizes);
        }

        QMessageBox::warning(this, protocolCreatorVersion, "Возникли проблемы при обработке документа.\nПожалуйста, проверьте логи");
    }
}

void MainWindow::on_actionFind_triggered()
{
    if (searchDialog)
    {
        delete searchDialog;
        searchDialog=0;
    }

    searchDialog = new SearchDialog(this);
    searchDialog->show();
}

void MainWindow::on_actionReplace_triggered()
{
    if (searchDialog)
    {
        delete searchDialog;
        searchDialog=0;
    }

    searchDialog = new SearchDialog(this);
    searchDialog->ui->replaceGroupBox->setChecked(true);
    searchDialog->show();
}

void MainWindow::on_actionFindNext_triggered()
{

}

void MainWindow::on_actionFindPrev_triggered()
{

}

void MainWindow::on_actionGlobalVars_triggered()
{
    globalDialog->exec();
}

void MainWindow::on_actionSectionControl_triggered()
{
    SectionControlDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionExport_triggered()
{
    on_actionCheckDocument_triggered();

    if (!errorHappened)
    {
        return;
    }
}

void MainWindow::on_actionGenerateWord_triggered()
{
    on_actionCheckDocument_triggered();

    if (!errorHappened)
    {
        return;
    }
}

void MainWindow::on_actionLogin_triggered()
{
    if (isAdmin)
    {
        isAdmin=false;
        updateAdmin();
    }
    else
    {
        if (adminPass.length()<3)
        {
            isAdmin=true;
            updateAdmin();
        }
        else
        {
            PasswordDialog dialog(this);
            dialog.ui->titleLabel->setText("Введите пароль администратора");
            dialog.ui->okButton->setText("Вход");

            if (dialog.exec())
            {
                if (EncryptString(dialog.ui->passwordEdit->text(), "Superman")==adminPass)
                {
                    isAdmin=true;
                    updateAdmin();
                }
                else
                {
                    QMessageBox::information(this, "Ввод пароля", "Неверный пароль");
                }
            }
        }
    }
}

void MainWindow::on_actionSetDocPass_triggered()
{
    if (!isAdmin)
    {
        return;
    }

    PasswordDialog dialog(this);
    dialog.ui->titleLabel->setText("Введите пароль на документ");

    if (dialog.exec())
    {
        docPass=EncryptString(dialog.ui->passwordEdit->text(), "Earthquake");
    }
}

void MainWindow::on_actionSetAdminPass_triggered()
{
    if (!isAdmin)
    {
        return;
    }

    PasswordDialog dialog(this);
    dialog.ui->titleLabel->setText("Введите пароль администратора");

    if (dialog.exec())
    {
        adminPass=EncryptString(dialog.ui->passwordEdit->text(), "Superman");
    }
}

void MainWindow::on_actionAddPage_triggered()
{
    if (!isAdmin)
    {
        return;
    }

    addPage("Новый раздел", "Section"+QString::number(ui->pagesTabWidget->count()+1));

    ui->pagesTabWidget->setCurrentIndex(ui->pagesTabWidget->count()-1);
}

void MainWindow::on_actionVariableInteger_triggered()
{
    if (!isAdmin || ui->pagesTabWidget->currentWidget()==contentPage)
    {
        return;
    }

    ((PageFrame*)ui->pagesTabWidget->currentWidget())->addVariable(new VariableIntegerFrame(ui->pagesTabWidget->currentWidget()));
}

void MainWindow::on_actionVariableString_triggered()
{
    if (!isAdmin || ui->pagesTabWidget->currentWidget()==contentPage)
    {
        return;
    }

    ((PageFrame*)ui->pagesTabWidget->currentWidget())->addVariable(new VariableStringFrame(ui->pagesTabWidget->currentWidget()));
}

void MainWindow::on_actionVariableBool_triggered()
{
    if (!isAdmin || ui->pagesTabWidget->currentWidget()==contentPage)
    {
        return;
    }

    ((PageFrame*)ui->pagesTabWidget->currentWidget())->addVariable(new VariableBoolFrame(ui->pagesTabWidget->currentWidget()));
}

void MainWindow::on_actionVariableDate_triggered()
{
    if (!isAdmin || ui->pagesTabWidget->currentWidget()==contentPage)
    {
        return;
    }

    ((PageFrame*)ui->pagesTabWidget->currentWidget())->addVariable(new VariableDateFrame(ui->pagesTabWidget->currentWidget()));
}

void MainWindow::on_actionVariableTime_triggered()
{
    if (!isAdmin || ui->pagesTabWidget->currentWidget()==contentPage)
    {
        return;
    }

    ((PageFrame*)ui->pagesTabWidget->currentWidget())->addVariable(new VariableTimeFrame(ui->pagesTabWidget->currentWidget()));
}

void MainWindow::on_actionVariableList_triggered()
{
    if (!isAdmin || ui->pagesTabWidget->currentWidget()==contentPage)
    {
        return;
    }

    ((PageFrame*)ui->pagesTabWidget->currentWidget())->addVariable(new VariableListFrame(ui->pagesTabWidget->currentWidget()));
}

void MainWindow::on_actionVariableExtendedList_triggered()
{
    if (!isAdmin || ui->pagesTabWidget->currentWidget()==contentPage)
    {
        return;
    }

    VariableExtendedListFrame *aVar=new VariableExtendedListFrame(ui->pagesTabWidget->currentWidget());

    aVar->mIsTable=false;
    aVar->ui->useCheckBox->setVisible(false);

    ((PageFrame*)ui->pagesTabWidget->currentWidget())->addVariable(aVar);
}

void MainWindow::on_actionVariableExpression_triggered()
{
    if (!isAdmin || ui->pagesTabWidget->currentWidget()==contentPage)
    {
        return;
    }

    ((PageFrame*)ui->pagesTabWidget->currentWidget())->addVariable(new VariableExpressionFrame(ui->pagesTabWidget->currentWidget()));
}

void MainWindow::on_actionComponentText_triggered()
{
    if (!isAdmin || ui->pagesTabWidget->currentWidget()==contentPage)
    {
        return;
    }

    ((PageFrame*)ui->pagesTabWidget->currentWidget())->addComponent(new ComponentTextFrame(ui->pagesTabWidget->currentWidget()));
}

void MainWindow::on_actionComponentTable_triggered()
{
    if (!isAdmin || ui->pagesTabWidget->currentWidget()==contentPage)
    {
        return;
    }

    VariableExtendedListFrame *aVar=new VariableExtendedListFrame(ui->pagesTabWidget->currentWidget());

    aVar->mIsTable=true;
    aVar->ui->titleLabel->setVisible(false);
    aVar->ui->nameEdit->setText("Таблица");
    aVar->ui->varNameEdit->setText("Table");
    aVar->ui->editButton->setFlat(false);

    ((PageFrame*)ui->pagesTabWidget->currentWidget())->addComponent(aVar);
}

void MainWindow::pageMoved(int from, int to)
{
    QWidget* aWidget=contentPage->ui->componentLayout->itemAt(from)->widget();

    contentPage->ui->componentLayout->removeWidget(aWidget);
    contentPage->ui->componentLayout->insertWidget(to, aWidget);
}

void MainWindow::page_nameChanged(PageFrame *parentPage)
{
    int pageIndex=ui->pagesTabWidget->indexOf(parentPage);

    if (pageIndex<0)
    {
        return;
    }

    ui->pagesTabWidget->setTabText(pageIndex, parentPage->ui->nameEdit->text());
    ((KnownCheckBox*)contentPage->ui->componentLayout->itemAt(pageIndex)->widget())->setText(parentPage->ui->nameEdit->text());
}

void MainWindow::page_useToggled(PageFrame *parentPage)
{
    int pageIndex=ui->pagesTabWidget->indexOf(parentPage);

    if (pageIndex<0)
    {
        return;
    }

    ((KnownCheckBox*)contentPage->ui->componentLayout->itemAt(pageIndex)->widget())->setChecked(parentPage->ui->useCheckBox->isChecked());
}

void MainWindow::contentCheckBoxToggled(KnownCheckBox *aCheckBox, bool checked)
{
    int pageIndex=contentPage->ui->componentLayout->indexOf(aCheckBox);

    if (pageIndex<0)
    {
        return;
    }

    ((PageFrame *)ui->pagesTabWidget->widget(pageIndex))->ui->useCheckBox->setChecked(checked);
}

void MainWindow::on_pagesTabWidget_tabCloseRequested(int index)
{
    if (ui->pagesTabWidget->widget(index)!=contentPage)
    {
        if (QMessageBox::question(this, protocolCreatorVersion, "Вы хотите удалить раздел \""+ui->pagesTabWidget->tabText(index)+"\"", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::Yes)
        {
            QWidget* aWidget=contentPage->ui->componentLayout->itemAt(index)->widget();
            contentPage->ui->componentLayout->removeWidget(aWidget);
            delete aWidget;

            aWidget=ui->pagesTabWidget->widget(index);
            ui->pagesTabWidget->removeTab(index);
            delete aWidget;
        }
    }
}

void MainWindow::addPage(QString aName, QString aVarName)
{
    PageFrame* aNewPage=new PageFrame(this);
    ui->pagesTabWidget->addTab(aNewPage, aName);

    if (contentPage==0)
    {
        contentPage=aNewPage;
    }

    KnownCheckBox *aCheckBox=new KnownCheckBox(contentPage);
    aCheckBox->setText(aName);
    aCheckBox->setChecked(true);
    contentPage->ui->componentLayout->addWidget(aCheckBox);

    aNewPage->ui->nameEdit->setText(aName);
    aNewPage->ui->varNameEdit->setText(aVarName);

    connect(aCheckBox, SIGNAL(checked(KnownCheckBox*,bool)), this, SLOT(contentCheckBoxToggled(KnownCheckBox*,bool)));
    connect(aNewPage, SIGNAL(nameChanged(PageFrame*)), this, SLOT(page_nameChanged(PageFrame*)));
    connect(aNewPage, SIGNAL(useToggled(PageFrame*)), this, SLOT(page_useToggled(PageFrame*)));
}

void MainWindow::addError(QString aText)
{
    ui->logListWidget->addItem(new QListWidgetItem(QIcon(":/images/Error.png"), aText));

    errorHappened=true;
}

void MainWindow::addHint(QString aText)
{
    ui->logListWidget->addItem(new QListWidgetItem(QIcon(":/images/Hint.png"), aText));
}

void MainWindow::addLog(QString aFullVarName, QString aText)
{
    bool isError=false;

    if (aText.startsWith("Error: "))
    {
        aText.remove(0, 7);
        isError=true;
    }
    else
    if (aText.startsWith("Hint: "))
    {
        aText.remove(0, 6);
    }

    if (isError)
    {
        addError(aFullVarName+": "+aText);
    }
    else
    {
        addHint(aFullVarName+": "+aText);
    }
}

void MainWindow::updateHeader()
{
    if (currentName=="")
    {
        setWindowTitle(protocolCreatorVersion);
    }
    else
    {
        setWindowTitle(protocolCreatorVersion+" ["+currentName.mid(currentName.lastIndexOf("/")+1)+"]");
    }
}

void MainWindow::updateAdmin()
{
    if (globalDialog)
    {
        globalDialog->updateAdmin();
    }

    for (int i=0; i<ui->pagesTabWidget->count(); i++)
    {
        ((PageFrame*)ui->pagesTabWidget->widget(i))->updateAdmin();
    }

    contentPage->ui->hideWidget->setVisible(false);

    ui->pagesTabWidget->setTabsClosable(isAdmin);
    ui->pagesTabWidget->setMovable(isAdmin);

    if (isAdmin)
    {
        ui->actionLogin->setText("Выйти из под администратора");
    }
    else
    {
        ui->actionLogin->setText("Войти под администратором");
    }

    ui->actionSetDocPass->setVisible(isAdmin);
    ui->actionSetAdminPass->setVisible(isAdmin);
    ui->menuSection->setEnabled(isAdmin);
}

void MainWindow::saveState()
{
    QDir(dir).mkpath(dir+"data");
    QFile::remove(dir+"data/Config.ini");
    QSettings aSettings(dir+"data/Config.ini",QSettings::IniFormat);

    aSettings.beginGroup("States");

    aSettings.setValue("Geometry",saveGeometry());
    aSettings.setValue("Divider_state",dividerSplitter->saveState());

    aSettings.endGroup();
}

void MainWindow::loadState()
{
    QSettings aSettings(dir+"data/Config.ini",QSettings::IniFormat);
    aSettings.beginGroup("States");

    restoreGeometry(aSettings.value("Geometry").toByteArray());
    dividerSplitter->restoreState(aSettings.value("Divider_state").toByteArray());

    aSettings.endGroup();
}
