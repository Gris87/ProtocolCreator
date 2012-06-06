#include "src/other/global.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mainWindow=this;

    ui->setupUi(this);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    dividerSplitter = new QSplitter(Qt::Vertical, this);
    ui->dividerLayout->removeWidget(ui->pagesTabWidget);
    ui->dividerLayout->removeWidget(ui->logListWidget);

    dividerSplitter->addWidget(ui->pagesTabWidget);
    dividerSplitter->addWidget(ui->logListWidget);

    QList<int> aSizes;

    aSizes.append(1);
    aSizes.append(0);

    dividerSplitter->setSizes(aSizes);

    ui->dividerLayout->addWidget(dividerSplitter);

    connect(ui->pagesTabWidget->tabBar(), SIGNAL(tabMoved(int,int)), this, SLOT(pageMoved(int,int)));
    connect(&autoSaveTimer, SIGNAL(timeout()), this, SLOT(autoSaveTick()));

    wantAutoSave=true;
    autoSaveMode=false;

    loadState();

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
        if (currentName!="")
        {
            autoSaveTick();
        }

        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::on_actionNew_triggered()
{
    if (currentName!="")
    {
        autoSaveTick();
    }

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

    resetAutoSaveTimer();

    autoSaveTick();
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog(this, protocolCreatorVersion, currentName, "Protocol creator file (*.pcr)");

    if (dialog.exec())
    {
        openFile(dialog.selectedFiles().at(0));
    }
}

void MainWindow::openFile(QString aFileName)
{
    QByteArray aArray;

    QFile aFile(aFileName);
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

    if (currentName!="")
    {
        autoSaveTick();
    }

    currentName=aFileName;
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

    ui->pagesTabWidget->setTabsClosable(false);

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

    resetAutoSaveTimer();

    autoSaveTick();
}

void MainWindow::on_actionSave_triggered()
{
    QString oldCurrentName=currentName;

    if (autoSaveMode)
    {
        QDir(dir).mkpath(dir+"autosave");

        if (currentName=="")
        {
            currentName=dir+"autosave/Untitled.pcr";
        }
        else
        {
            QString baseName=currentName.mid(currentName.lastIndexOf("/")+1);
            baseName=baseName.left(baseName.lastIndexOf("."));

            currentName=dir+"autosave/"+baseName+".pcr";
        }
    }

    if (currentName=="" || (!autoSaveMode && currentName.startsWith(dir+"autosave/")))
    {
        if (currentName!="")
        {
            QMessageBox::information(this, protocolCreatorVersion, "Пожалуйста, сохраните в другом месте. Сохранять в папке autosave запрещено");
        }

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

        if (globalDialog)
        {
            globalDialog->saveToStream(aStream);
        }

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
        if (!autoSaveMode && aFile.exists())
        {
            QDir(dir).mkpath(dir+"backup");

            QString baseName=currentName.mid(currentName.lastIndexOf("/")+1);
            baseName=baseName.left(baseName.lastIndexOf("."));

            QString backupName=dir+"backup/"+baseName+"_"+QFileInfo(aFile).lastModified().toString("yyyy_MM_dd_hh_mm_ss_zzz")+".pcr";

            if (QFile::exists(backupName))
            {
                QFile::remove(backupName);
            }

            aFile.copy(backupName);
        }

        aFile.open(QIODevice::WriteOnly);
        aFile.write(aArray);
        aFile.close();

        if (!autoSaveMode)
        {
            resetAutoSaveTimer();
        }
    }

    if (autoSaveMode)
    {
        currentName=oldCurrentName;
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QFileDialog dialog(this, protocolCreatorVersion, currentName, "Protocol creator file (*.pcr)");
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
                globalDialog->variables.at(i)->calculate(&aErrors);
            }
            catch(...)
            {
                addError("Global."+globalDialog->variables.at(i)->variableName()+": "+globalDialog->variables.at(i)->calculationError);
            }

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
                try
                {
                    aPage->variables.at(j)->calculate(&aErrors);
                }
                catch(...)
                {
                    addError(aPage->ui->varNameEdit->text()+"."+aPage->variables.at(j)->variableName()+": "+aPage->variables.at(j)->calculationError);
                }

                for (int k=0; k<aErrors.length(); k++)
                {
                    addLog(aPage->ui->varNameEdit->text()+"."+aPage->variables.at(j)->variableName(), aErrors.at(k));
                }

                aErrors.clear();

                ui->progressBar->setValue(ui->progressBar->value()+1);
            }

            for (int j=0; j<aPage->components.length(); j++)
            {
                try
                {
                    aPage->components.at(j)->calculate(&aErrors);
                }
                catch(...)
                {
                    addError(aPage->ui->varNameEdit->text()+"."+aPage->components.at(j)->variableName()+": "+aPage->components.at(j)->calculationError);
                }

                for (int k=0; k<aErrors.length(); k++)
                {
                    addLog(aPage->ui->varNameEdit->text()+"."+aPage->components.at(j)->variableName(), aErrors.at(k));
                }

                aErrors.clear();

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

        if (errorHappened)
        {
            QMessageBox::warning(this, protocolCreatorVersion, "Возникли проблемы при обработке документа.\nПожалуйста, проверьте логи");
        }
    }
}

void MainWindow::on_actionFind_triggered()
{
    if (searchDialog)
    {
        delete searchDialog;
    }

    if (fullDialog)
    {
        searchDialog = new SearchDialog(fullDialog);
    }
    else
    {
        if (globalDialog->isVisible())
        {
            searchDialog = new SearchDialog(globalDialog);
        }
        else
        {
            searchDialog = new SearchDialog(this);
        }
    }

    searchDialog->show();
}

void MainWindow::on_actionReplace_triggered()
{
    if (searchDialog)
    {
        delete searchDialog;
    }

    if (fullDialog)
    {
        searchDialog = new SearchDialog(fullDialog);
    }
    else
    {
        if (globalDialog->isVisible())
        {
            searchDialog = new SearchDialog(globalDialog);
        }
        else
        {
            searchDialog = new SearchDialog(this);
        }
    }

    searchDialog->ui->replaceGroupBox->setChecked(true);
    searchDialog->show();
}

void MainWindow::on_actionFindNext_triggered()
{
    if (lastSearch=="")
    {
        on_actionFind_triggered();
        return;
    }

    if (isFindAll)
    {
        if (fullDialog && fullDialog->pageComponent)
        {
            QWidget *aWidget=0;

            if (fullDialog->pageComponent->inherits("ComponentTextFrame"))
            {
                aWidget=((ComponentTextFrame*)(fullDialog->pageComponent))->wordEdit->ui->valueEdit;
            }
            else
            if (fullDialog->pageComponent->inherits("VariableExtendedListFrame"))
            {
                aWidget=((VariableExtendedListFrame*)(fullDialog->pageComponent))->ui->dataTableWidget;
            }

            aWidget->setFocus();
            fullDialog->pageComponent->setWidgetCursor(aWidget, true);
        }
        else
        {
            bool good=false;

            if (globalDialog->isVisible())
            {
                for (int i=0; i<globalDialog->variables.length(); i++)
                {
                    QList<QWidget *> widgets;
                    globalDialog->variables.at(i)->getWidgetList(widgets);

                    if (widgets.length()>0)
                    {
                        widgets.first()->setFocus();
                        globalDialog->variables.at(i)->setWidgetCursor(widgets.first(), true);
                        good=true;
                        break;
                    }
                }
            }

            if (!good)
            {
                for (int i=0; i<ui->pagesTabWidget->count(); i++)
                {
                    PageFrame *aPage=(PageFrame*)ui->pagesTabWidget->widget(i);

                    for (int j=0; j<aPage->variables.length(); j++)
                    {
                        QList<QWidget *> widgets;
                        aPage->variables.at(j)->getWidgetList(widgets);

                        if (widgets.length()>0)
                        {
                            widgets.first()->setFocus();
                            aPage->variables.at(j)->setWidgetCursor(widgets.first(), true);
                            good=true;
                            break;
                        }
                    }

                    if (good)
                    {
                        break;
                    }

                    for (int j=0; j<aPage->components.length(); j++)
                    {
                        QList<QWidget *> widgets;
                        aPage->components.at(j)->getWidgetList(widgets);

                        if (widgets.length()>0)
                        {
                            widgets.first()->setFocus();
                            aPage->components.at(j)->setWidgetCursor(widgets.first(), true);
                            good=true;
                            break;
                        }
                    }

                    if (good)
                    {
                        break;
                    }
                }
            }
        }
    }

    int startPage;
    int varIndex;
    int compIndex;

    findFocus(startPage, varIndex, compIndex);

    int startIndex;
    int totalCount;

    int componentStartX=-1;
    int componentStartY=-1;

    if (startPage==-2)
    {
        totalCount=0;

        if (fullDialog->pageComponent->inherits("ComponentTextFrame"))
        {
            componentStartX=((ComponentTextFrame*)(fullDialog->pageComponent))->wordEdit->ui->valueEdit->textCursor().position();
        }
        else
        if (fullDialog->pageComponent->inherits("VariableExtendedListFrame"))
        {
            componentStartX=((VariableExtendedListFrame*)(fullDialog->pageComponent))->ui->dataTableWidget->currentColumn();
            componentStartY=((VariableExtendedListFrame*)(fullDialog->pageComponent))->ui->dataTableWidget->currentRow();
        }

        if (componentStartX<0)
        {
            componentStartX=0;
        }

        if (componentStartY<0)
        {
            componentStartY=0;
        }
    }
    else
    if (startPage==-1)
    {
        totalCount=globalDialog->variables.length();
    }
    else
    {
        PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(startPage);

        totalCount=aPage->variables.length()+aPage->components.length();
    }

    if (startPage==-2 || varIndex>=0)
    {
        startIndex=varIndex;
    }
    else
    {
        PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(startPage);

        if (compIndex>=0)
        {
            startIndex=aPage->variables.length()+compIndex;
        }
        else
        {
            startIndex=0;
        }
    }

    int curPage=startPage;
    int curIndex=startIndex;

    do
    {
        if (curIndex<totalCount)
        {
            PageComponent *aComponent;

            if (fullDialog && fullDialog->pageComponent)
            {
                aComponent=fullDialog->pageComponent;
            }
            else
            {
                if (curPage==-1)
                {
                    aComponent=globalDialog->variables.at(curIndex);
                }
                else
                {
                    PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(curPage);

                    if (curIndex<aPage->variables.length())
                    {
                        aComponent=aPage->variables.at(curIndex);
                    }
                    else
                    {
                        aComponent=aPage->components.at(curIndex-aPage->variables.length());
                    }
                }
            }

            if (
                curPage==-2
                ||
                (
                 curPage==-1
                 &&
                 aComponent->isVisibleTo(globalDialog)
                )
                ||
                (
                 curPage>=0
                 &&
                 aComponent->isVisibleTo(mainWindow->ui->pagesTabWidget->widget(curPage))
                )
               )
            {
                if (aComponent->find(true, componentStartX, componentStartY))
                {
                    if (isFindAll)
                    {
                        continue;
                    }
                    else
                    {
                        if (curPage==-2)
                        {
                        }
                        else
                        if (curPage==-1)
                        {
                            globalDialog->ui->scrollArea->ensureWidgetVisible(globalDialog->focusWidget());

                            if (!globalDialog->isVisible())
                            {
                                if (searchDialog)
                                {
                                    globalDialog->show();

                                    SearchDialog* oldSearchDialog=searchDialog;
                                    searchDialog=new SearchDialog(globalDialog);
                                    searchDialog->ui->replaceGroupBox->setChecked(oldSearchDialog->ui->replaceGroupBox->isChecked());
                                    searchDialog->setGeometry(oldSearchDialog->geometry());
                                    searchDialog->show();

                                    delete oldSearchDialog;

                                    globalDialog->activateWindow();
                                }

                                globalDialog->exec();
                            }
                        }
                        else
                        {
                            if (globalDialog->isVisible())
                            {
                                globalDialog->reject();
                            }

                            mainWindow->ui->pagesTabWidget->setCurrentIndex(curPage);

                            ((PageFrame*)mainWindow->ui->pagesTabWidget->widget(curPage))->ui->scrollArea->ensureWidgetVisible(mainWindow->focusWidget());
                        }

                        break;
                    }
                }
            }
        }

        if (curPage>=-1)
        {
            curIndex++;

            if (curIndex>=totalCount)
            {
                curIndex=0;
                curPage++;

                if (curPage>=ui->pagesTabWidget->count())
                {
                    curPage=-1;
                }

                if (curPage==-1)
                {
                    totalCount=globalDialog->variables.length();
                }
                else
                {
                    PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(curPage);

                    totalCount=aPage->variables.length()+aPage->components.length();
                }
            }
        }

        if (curPage==startPage && curIndex==startIndex)
        {
            break;
        }

        if (curIndex<totalCount)
        {
            PageComponent *aComponent;

            if (fullDialog && fullDialog->pageComponent)
            {
                aComponent=fullDialog->pageComponent;
            }
            else
            {
                if (curPage==-1)
                {
                    aComponent=globalDialog->variables.at(curIndex);
                }
                else
                {
                    PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(curPage);

                    if (curIndex<aPage->variables.length())
                    {
                        aComponent=aPage->variables.at(curIndex);
                    }
                    else
                    {
                        aComponent=aPage->components.at(curIndex-aPage->variables.length());
                    }
                }
            }

            if (
                curPage==-2
                ||
                (
                 curPage==-1
                 &&
                 aComponent->isVisibleTo(globalDialog)
                )
                ||
                (
                 curPage>=0
                 &&
                 aComponent->isVisibleTo(mainWindow->ui->pagesTabWidget->widget(curPage))
                )
               )
            {
                QList<QWidget*> widgets;
                aComponent->getWidgetList(widgets);

                if (widgets.length()>0)
                {
                    widgets.first()->setFocus();
                    aComponent->setWidgetCursor(widgets.first(), true);
                }
            }
        }
    }
    while (true);
}

void MainWindow::on_actionFindPrev_triggered()
{
    if (lastSearch=="")
    {
        on_actionFind_triggered();
        return;
    }

    if (isFindAll)
    {
        if (fullDialog && fullDialog->pageComponent)
        {
            QWidget *aWidget=0;

            if (fullDialog->pageComponent->inherits("ComponentTextFrame"))
            {
                aWidget=((ComponentTextFrame*)(fullDialog->pageComponent))->wordEdit->ui->valueEdit;
            }
            else
            if (fullDialog->pageComponent->inherits("VariableExtendedListFrame"))
            {
                aWidget=((VariableExtendedListFrame*)(fullDialog->pageComponent))->ui->dataTableWidget;
            }

            aWidget->setFocus();
            fullDialog->pageComponent->setWidgetCursor(aWidget, false);
        }
        else
        {
            bool good=false;

            if (globalDialog->isVisible())
            {
                for (int i=0; i<globalDialog->variables.length(); i++)
                {
                    QList<QWidget *> widgets;
                    globalDialog->variables.at(i)->getWidgetList(widgets);

                    if (widgets.length()>0)
                    {
                        widgets.last()->setFocus();
                        globalDialog->variables.at(i)->setWidgetCursor(widgets.first(), true);
                        good=true;
                        break;
                    }
                }
            }

            if (!good)
            {
                for (int i=0; i<ui->pagesTabWidget->count(); i++)
                {
                    PageFrame *aPage=(PageFrame*)ui->pagesTabWidget->widget(i);

                    for (int j=0; j<aPage->variables.length(); j++)
                    {
                        QList<QWidget *> widgets;
                        aPage->variables.at(j)->getWidgetList(widgets);

                        if (widgets.length()>0)
                        {
                            widgets.last()->setFocus();
                            aPage->variables.at(j)->setWidgetCursor(widgets.first(), true);
                            good=true;
                            break;
                        }
                    }

                    if (good)
                    {
                        break;
                    }

                    for (int j=0; j<aPage->components.length(); j++)
                    {
                        QList<QWidget *> widgets;
                        aPage->components.at(j)->getWidgetList(widgets);

                        if (widgets.length()>0)
                        {
                            widgets.last()->setFocus();
                            aPage->components.at(j)->setWidgetCursor(widgets.first(), true);
                            good=true;
                            break;
                        }
                    }

                    if (good)
                    {
                        break;
                    }
                }
            }
        }
    }

    int startPage;
    int varIndex;
    int compIndex;

    findFocus(startPage, varIndex, compIndex);

    int startIndex;
    int totalCount;

    int componentStartX=-1;
    int componentStartY=-1;

    if (startPage==-2)
    {
        totalCount=0;

        if (fullDialog->pageComponent->inherits("ComponentTextFrame"))
        {
            componentStartX=((ComponentTextFrame*)(fullDialog->pageComponent))->wordEdit->ui->valueEdit->textCursor().position();
            componentStartY=0;

            if (componentStartX<0)
            {
                componentStartX=((ComponentTextFrame*)(fullDialog->pageComponent))->wordEdit->ui->valueEdit->document()->rootFrame()->lastCursorPosition().position();
            }
        }
        else
        if (fullDialog->pageComponent->inherits("VariableExtendedListFrame"))
        {
            componentStartX=((VariableExtendedListFrame*)(fullDialog->pageComponent))->ui->dataTableWidget->currentColumn();
            componentStartY=((VariableExtendedListFrame*)(fullDialog->pageComponent))->ui->dataTableWidget->currentRow();

            if (componentStartX<0)
            {
                componentStartX=((VariableExtendedListFrame*)(fullDialog->pageComponent))->ui->dataTableWidget->columnCount()-1;
            }

            if (componentStartY<0)
            {
                componentStartY=((VariableExtendedListFrame*)(fullDialog->pageComponent))->ui->dataTableWidget->rowCount()-1;
            }
        }
    }
    else
    if (startPage==-1)
    {
        totalCount=globalDialog->variables.length();
    }
    else
    {
        PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(startPage);

        totalCount=aPage->variables.length()+aPage->components.length();
    }

    if (startPage==-2 || varIndex>=0)
    {
        startIndex=varIndex;
    }
    else
    {
        PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(startPage);

        if (compIndex>=0)
        {
            startIndex=aPage->variables.length()+compIndex;
        }
        else
        {
            startIndex=totalCount-1;

            if (startIndex<0)
            {
                startIndex=0;
            }
        }
    }

    int curPage=startPage;
    int curIndex=startIndex;

    do
    {
        if (curIndex<totalCount)
        {
            PageComponent *aComponent;

            if (fullDialog && fullDialog->pageComponent)
            {
                aComponent=fullDialog->pageComponent;
            }
            else
            {
                if (curPage==-1)
                {
                    aComponent=globalDialog->variables.at(curIndex);
                }
                else
                {
                    PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(curPage);

                    if (curIndex<aPage->variables.length())
                    {
                        aComponent=aPage->variables.at(curIndex);
                    }
                    else
                    {
                        aComponent=aPage->components.at(curIndex-aPage->variables.length());
                    }
                }
            }

            if (
                curPage==-2
                ||
                (
                 curPage==-1
                 &&
                 aComponent->isVisibleTo(globalDialog)
                )
                ||
                (
                 curPage>=0
                 &&
                 aComponent->isVisibleTo(mainWindow->ui->pagesTabWidget->widget(curPage))
                )
               )
            {
                if (aComponent->find(false, componentStartX, componentStartY))
                {
                    if (isFindAll)
                    {
                        continue;
                    }
                    else
                    {
                        if (curPage==-2)
                        {
                        }
                        else
                        if (curPage==-1)
                        {
                            globalDialog->ui->scrollArea->ensureWidgetVisible(globalDialog->focusWidget());

                            if (!globalDialog->isVisible())
                            {
                                if (searchDialog)
                                {
                                    globalDialog->show();

                                    SearchDialog* oldSearchDialog=searchDialog;
                                    searchDialog=new SearchDialog(globalDialog);
                                    searchDialog->ui->replaceGroupBox->setChecked(oldSearchDialog->ui->replaceGroupBox->isChecked());
                                    searchDialog->setGeometry(oldSearchDialog->geometry());
                                    searchDialog->show();

                                    delete oldSearchDialog;

                                    globalDialog->activateWindow();
                                }

                                globalDialog->exec();
                            }
                        }
                        else
                        {
                            if (globalDialog->isVisible())
                            {
                                globalDialog->reject();
                            }

                            mainWindow->ui->pagesTabWidget->setCurrentIndex(curPage);

                            ((PageFrame*)mainWindow->ui->pagesTabWidget->widget(curPage))->ui->scrollArea->ensureWidgetVisible(mainWindow->focusWidget());
                        }

                        break;
                    }
                }
            }
        }

        if (curPage>=-1)
        {
            curIndex--;

            if (curIndex<0)
            {
                curPage--;

                if (curPage<-1)
                {
                    curPage=ui->pagesTabWidget->count()-1;
                }

                if (curPage==-1)
                {
                    totalCount=globalDialog->variables.length();
                }
                else
                {
                    PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(curPage);

                    totalCount=aPage->variables.length()+aPage->components.length();
                }

                curIndex=totalCount-1;

                if (curIndex<0)
                {
                    curIndex=0;
                }
            }
        }

        if (curPage==startPage && curIndex==startIndex)
        {
            break;
        }

        if (curIndex<totalCount)
        {
            PageComponent *aComponent;

            if (fullDialog && fullDialog->pageComponent)
            {
                aComponent=fullDialog->pageComponent;
            }
            else
            {
                if (curPage==-1)
                {
                    aComponent=globalDialog->variables.at(curIndex);
                }
                else
                {
                    PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(curPage);

                    if (curIndex<aPage->variables.length())
                    {
                        aComponent=aPage->variables.at(curIndex);
                    }
                    else
                    {
                        aComponent=aPage->components.at(curIndex-aPage->variables.length());
                    }
                }
            }

            if (
                curPage==-2
                ||
                (
                 curPage==-1
                 &&
                 aComponent->isVisibleTo(globalDialog)
                )
                ||
                (
                 curPage>=0
                 &&
                 aComponent->isVisibleTo(mainWindow->ui->pagesTabWidget->widget(curPage))
                )
               )
            {
                QList<QWidget*> widgets;
                aComponent->getWidgetList(widgets);

                if (widgets.length()>0)
                {
                    widgets.last()->setFocus();
                    aComponent->setWidgetCursor(widgets.last(), false);
                }
            }
        }
    }
    while (true);
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

void MainWindow::exportToWord(bool isFull)
{
    QFileDialog dialog(this, QString(), QString(), "Word document (*.doc *.docx *.xml)");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setWindowTitle("Сохранить документ Word");
    dialog.setConfirmOverwrite(false);

    QString aFileName;

    do
    {
        if (dialog.exec())
        {
            aFileName=dialog.selectedFiles().at(0);

            if (aFileName.endsWith(".doc"))
            {
                aFileName.remove(aFileName.length()-4,4);
            }
            else
            if (aFileName.endsWith(".docx"))
            {
                aFileName.remove(aFileName.length()-5,5);
            }
            else
            if (aFileName.endsWith(".xml"))
            {
                if (QFile::exists(aFileName))
                {
                    if (QMessageBox::question(this, protocolCreatorVersion, "Вы хотите заменить \""+QDir::toNativeSeparators(aFileName)+"\"?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::No)
                    {
                        continue;
                    }
                }

                break;
            }

            if (QFile::exists(aFileName+".doc"))
            {
                if (QMessageBox::question(this, protocolCreatorVersion, "Вы хотите заменить \""+QDir::toNativeSeparators(aFileName)+".doc\"?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::No)
                {
                    continue;
                }
            }

            if (QFile::exists(aFileName+".docx"))
            {
                if (QMessageBox::question(this, protocolCreatorVersion, "Вы хотите заменить \""+QDir::toNativeSeparators(aFileName)+".docx\"?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::No)
                {
                    continue;
                }
            }

            break;
        }
        else
        {
            return;
        }
    } while (true);



    on_actionCheckDocument_triggered();

    if (errorHappened)
    {
        return;
    }



    int logsBefore=ui->logListWidget->count();

    QString aTempPath=QDir::fromNativeSeparators(QString(getenv("Temp")));

    if (!aTempPath.endsWith("/"))
    {
        aTempPath.append("/");
    }

    int contentIndex=-1;
    bool good=true;

    try
    {
        WordXML word(2003);

        int start;
        int stop;

        if (isFull)
        {
            start=0;
            stop=ui->pagesTabWidget->count()-1;
        }
        else
        {
            start=ui->pagesTabWidget->currentIndex();
            stop=start;
        }

        int aCompCount=0;

        for (int i=start; i<=stop; i++)
        {
            PageFrame* aPage=(PageFrame*)ui->pagesTabWidget->widget(i);

            if (
                aPage!=contentPage
                &&
                (
                 !isFull
                 ||
                 aPage->ui->useCheckBox->isChecked()
                )
               )
            {
                aCompCount+=aPage->components.length();
            }
        }

        if (aCompCount==0)
        {
            aCompCount=1;
        }

        ui->progressBar->setMaximum(aCompCount);
        ui->progressBar->setValue(0);

        WordXMLSection *section=0;

        for (int i=start; i<=stop; i++)
        {
            PageFrame* aPage=(PageFrame*)ui->pagesTabWidget->widget(i);

            if (!isFull || aPage->ui->useCheckBox->isChecked())
            {
                // Необходимо перекинуть настройки секции в настройки параграфа, чтобы создать новую секцию
                if (section)
                {
                    WordXMLParagraph *paragraph=section->addParagraph();
                    paragraph->properties.sectionProperties=section->properties;
                    section->properties.clear();
                    paragraph->properties.sectionProperties.needToGenerate=true;
                    section->properties.needToGenerate=false;
                }

                section=word.sections.add();
                section->properties.landscape=!aPage->isPortaitOrientation;
                section->properties.setPageMargin((int)(aPage->topLimit*CM_TO_TWIPS),
                                                  (int)(aPage->leftLimit*CM_TO_TWIPS),
                                                  (int)(aPage->bottomLimit*CM_TO_TWIPS),
                                                  (int)(aPage->rightLimit*CM_TO_TWIPS));

                QTextEdit aTextEdit;

                aTextEdit.setHtml(aPage->headerText);
                replaceLinksInText(&aTextEdit, 0);
                section->properties.addHeader()->insertFromText(aTextEdit.document()->rootFrame());

                aTextEdit.setHtml(aPage->footerText);
                replaceLinksInText(&aTextEdit, 0);
                section->properties.addFooter()->insertFromText(aTextEdit.document()->rootFrame());

                if (aPage==contentPage)
                {
                    contentIndex=word.sections.count()-1;

                    QFont aFont;
                    aFont.setFamily("Times New Roman");
                    aFont.setPointSize(11);
                    aFont.setBold(true);

                    WordXMLTableRow *aRow;
                    WordXMLTableCell *aCell;
                    WordXMLParagraph *aParagraph;
                    WordXMLRun *aRun;

                    aParagraph=section->addParagraph();
                    aRun=aParagraph->addRun();

                    aParagraph->properties.alignment=paCenter;
                    aRun->properties.setFont(aFont);

                    aRun->addText("СОДЕРЖАНИЕ");

                    aParagraph=section->addParagraph();

                    aFont.setPointSize(10);

                    WordXMLTable *aTable=section->addTable();
                    aTable->properties.borders.setBorders(tbSingle);
                    aTable->properties.alignment=taCenter;

                    aRow=aTable->addRow();

                    aRow->properties.isHeader=true;

                    //---

                    aCell=aRow->addCell();
                    aCell->properties.width=459;
                    aCell->properties.vAlign=caCenter;

                    aParagraph=aCell->addParagraph();
                    aRun=aParagraph->addRun();

                    aParagraph->properties.alignment=paCenter;

                    aRun->properties.setFont(aFont);

                    aRun->addText("№");

                    aParagraph=aCell->addParagraph();
                    aRun=aParagraph->addRun();

                    aParagraph->properties.alignment=paCenter;
                    aRun->properties.setFont(aFont);

                    aRun->addText("п/п");

                    //---

                    aCell=aRow->addCell();
                    aCell->properties.width=6975;
                    aCell->properties.vAlign=caCenter;

                    aParagraph=aCell->addParagraph();
                    aRun=aParagraph->addRun();

                    aParagraph->properties.alignment=paCenter;
                    aRun->properties.setFont(aFont);

                    aRun->addText("Наименование");

                    //---

                    aCell=aRow->addCell();
                    aCell->properties.width=850;
                    aCell->properties.vAlign=caCenter;

                    aParagraph=aCell->addParagraph();
                    aRun=aParagraph->addRun();

                    aParagraph->properties.alignment=paCenter;
                    aRun->properties.setFont(aFont);

                    aRun->addText("Всего листов");

                    //---

                    aCell=aRow->addCell();
                    aCell->properties.width=1133;
                    aCell->properties.vAlign=caCenter;

                    aParagraph=aCell->addParagraph();
                    aRun=aParagraph->addRun();

                    aParagraph->properties.alignment=paCenter;
                    aRun->properties.setFont(aFont);

                    aRun->addText("Лист протокола");

                    //---

                    int sectionNumber=0;
                    aFont.setBold(false);

                    for (int j=0; j<ui->pagesTabWidget->count(); j++)
                    {
                        PageFrame* aPage2=(PageFrame*)ui->pagesTabWidget->widget(j);

                        if (aPage2->ui->useCheckBox->isChecked())
                        {
                            sectionNumber++;

                            aRow=aTable->addRow();

                            //---

                            aFont.setPointSize(11);

                            aCell=aRow->addCell();
                            aCell->properties.width=459;
                            aCell->properties.vAlign=caCenter;

                            aParagraph=aCell->addParagraph();
                            aRun=aParagraph->addRun();

                            aParagraph->properties.alignment=paCenter;

                            aRun->properties.setFont(aFont);

                            aRun->addText(QString::number(sectionNumber)+".");

                            //---

                            aFont.setPointSize(9);

                            aCell=aRow->addCell();
                            aCell->properties.width=6975;
                            aCell->properties.vAlign=caCenter;

                            QString aText=aPage2->ui->nameEdit->text();

                            do
                            {
                                QString aTextPart=aText.left(aText.indexOf("\n"));
                                aText.remove(0, aTextPart.length()+1);

                                aParagraph=aCell->addParagraph();
                                aRun=aParagraph->addRun();

                                aParagraph->properties.alignment=paLeft;
                                aParagraph->properties.spacingBefore=120;
                                aParagraph->properties.spacingAfter=120;
                                aParagraph->properties.indentLeft=280;
                                aRun->properties.setFont(aFont);

                                aRun->addText(aTextPart);
                            } while (aText!="");

                            //---

                            aCell=aRow->addCell();
                            aCell->properties.width=850;
                            aCell->properties.vAlign=caCenter;

                            aParagraph=aCell->addParagraph();
                            aRun=aParagraph->addRun();

                            aParagraph->properties.alignment=paCenter;
                            aRun->properties.setFont(aFont);

                            aRun->addText("");

                            //---

                            aCell=aRow->addCell();
                            aCell->properties.width=1133;
                            aCell->properties.vAlign=caCenter;

                            aParagraph=aCell->addParagraph();
                            aRun=aParagraph->addRun();

                            aParagraph->properties.alignment=paCenter;
                            aRun->properties.setFont(aFont);

                            aRun->addText("");
                        }
                    }
                }
                else
                {
                    for (int i=0; i<aPage->components.length(); i++)
                    {
                        if (aPage->components.at(i)->inherits("ComponentTextFrame"))
                        {
                            ComponentTextFrame *aComponent=(ComponentTextFrame*)aPage->components.at(i);

                            if (aComponent->ui->useCheckBox->isChecked())
                            {
                                aTextEdit.setHtml(aComponent->wordEdit->ui->valueEdit->toHtml());
                                replaceLinksInText(&aTextEdit, aComponent);
                                section->insertFromText(aTextEdit.document()->rootFrame());
                            }
                        }
                        else
                        if (aPage->components.at(i)->inherits("VariableExtendedListFrame"))
                        {
                            VariableExtendedListFrame *aComponent=(VariableExtendedListFrame*)aPage->components.at(i);

                            if (aComponent->ui->useCheckBox->isChecked())
                            {
                                WordXMLTable *aTable=section->addTable();
                                aTable->properties.borders.setBorders(tbSingle);

                                if (aComponent->mTableAlignment==Qt::AlignLeft)
                                {
                                    aTable->properties.alignment=taLeft;
                                }
                                else
                                if (aComponent->mTableAlignment==Qt::AlignHCenter || aComponent->mTableAlignment==Qt::AlignCenter)
                                {
                                    aTable->properties.alignment=taCenter;
                                }
                                else
                                if (aComponent->mTableAlignment==Qt::AlignRight)
                                {
                                    aTable->properties.alignment=taRight;
                                }

                                aTable->properties.indentation=(int)(aComponent->mTableOffset*CM_TO_TWIPS);

                                for (int i=0; i<aComponent->headerCells.length(); i++)
                                {
                                    WordXMLTableRow *aRow=aTable->addRow();

                                    if (aComponent->cloneHeader)
                                    {
                                        aRow->properties.isHeader=true;
                                    }

                                    for (int j=0; j<aComponent->headerCells.at(i).length(); j++)
                                    {
                                        STableCell aTableCell=aComponent->headerCells.at(i).at(j);
                                        WordXMLTableCell *aCell=aRow->addCell();

                                        aCell->properties.width=aComponent->headerColumnWidths.at(j);
                                        aCell->properties.columnSpan=aTableCell.spanX;

                                        if (aTableCell.alignment & Qt::AlignTop)
                                        {
                                            aCell->properties.vAlign=caTop;
                                        }
                                        else
                                        if (aTableCell.alignment & Qt::AlignVCenter)
                                        {
                                            aCell->properties.vAlign=caCenter;
                                        }
                                        else
                                        if (aTableCell.alignment & Qt::AlignBottom)
                                        {
                                            aCell->properties.vAlign=caBottom;
                                        }

                                        aCell->properties.shading.pattern="pct-25";
                                        aCell->properties.shading.setColor(QColor(aTableCell.backgroundColorR, aTableCell.backgroundColorG, aTableCell.backgroundColorB));
                                        aCell->properties.shading.backgroundColor=aCell->properties.shading.color;
                                        aCell->properties.shading.fillColor=aCell->properties.shading.color;

                                        for (int k=1; k<aCell->properties.columnSpan; k++)
                                        {
                                            aCell->properties.width+=aComponent->headerColumnWidths.at(j+k);
                                        }

                                        aCell->properties.width=aCell->properties.width*PIXELS_TO_TWIPS;

                                        bool writeCell=true;

                                        if (aTableCell.spanY>1)
                                        {
                                            bool needToContinue=false;

                                            for (int k=0; k<=i; k++)
                                            {
                                                if (aComponent->headerCells.at(k).at(j).spanY>1)
                                                {
                                                    needToContinue=(k<i) && (k+aComponent->headerCells.at(k).at(j).spanY>i);

                                                    if (needToContinue)
                                                    {
                                                        break;
                                                    }

                                                    k+=aComponent->headerCells.at(k).at(j).spanY-1;
                                                }
                                            }

                                            if (needToContinue)
                                            {
                                                aCell->properties.vMergeType=mtContinue;
                                            }
                                            else
                                            {
                                                aCell->properties.vMergeType=mtRestart;
                                            }
                                        }

                                        if (writeCell)
                                        {
                                            QString aText=aTableCell.text;

                                            do
                                            {
                                                WordXMLParagraph *aParagraph=aCell->addParagraph();
                                                WordXMLRun *aRun=aParagraph->addRun();


                                                if (aTableCell.alignment & Qt::AlignLeft)
                                                {
                                                    aParagraph->properties.alignment=paLeft;
                                                }
                                                else
                                                if (aTableCell.alignment & Qt::AlignHCenter)
                                                {
                                                    aParagraph->properties.alignment=paCenter;
                                                }
                                                else
                                                if (aTableCell.alignment & Qt::AlignRight)
                                                {
                                                    aParagraph->properties.alignment=paRight;
                                                }

                                                QFont aFont;
                                                aFont.fromString(aTableCell.fontString);

                                                aRun->properties.setFont(aFont);
                                                aRun->properties.setColor(QColor(aTableCell.textColorR, aTableCell.textColorG, aTableCell.textColorB));

                                                QString aTextPart=aText.left(aText.indexOf("\n"));
                                                aText.remove(0, aTextPart.length()+1);

                                                aRun->addText(aTextPart);
                                            } while (aText!="");
                                        }

                                        if (aCell->properties.columnSpan>1)
                                        {
                                            j+=aCell->properties.columnSpan-1;
                                        }
                                    }
                                }

                                QVariantList rowValue=aComponent->calculationResult.toList();

                                aTable->properties.width=0;

                                for (int i=0; i<aComponent->ui->dataTableWidget->rowCount(); i++)
                                {
                                    double rowWidth=0;

                                    QVariantList colValues=rowValue.at(i).toList();
                                    QList<int> separations;
                                    QList<int> separationsXMLColumn;

                                    WordXMLTableRow *aRow=aTable->addRow();

                                    int headerI=-1;

                                    for (int j=0; j<aComponent->typeColumns.length(); j++)
                                    {
                                        if (
                                            aComponent->typeColumns.at(j).column->type()==ctInteger
                                            &&
                                            !((IntegerColumn*)aComponent->typeColumns.at(j).column)->mIsAutoInc
                                            &&
                                            ((IntegerColumn*)aComponent->typeColumns.at(j).column)->mSplitRows
                                           )
                                        {
                                            separations.append(j);
                                            separationsXMLColumn.append(aRow->cellCount());
                                        }

                                        if (!aComponent->typeColumns.at(j).visible)
                                        {
                                            continue;
                                        }

                                        headerI++;

                                        WordXMLTableCell *aCell=aRow->addCell();

                                        aCell->properties.width=aComponent->headerColumnWidths.at(headerI);
                                        aCell->properties.columnSpan=aComponent->ui->dataTableWidget->columnSpan(i, j);

                                        if (aComponent->ui->dataTableWidget->item(i, j)->textAlignment() & Qt::AlignTop)
                                        {
                                            aCell->properties.vAlign=caTop;
                                        }
                                        else
                                        if (aComponent->ui->dataTableWidget->item(i, j)->textAlignment() & Qt::AlignVCenter)
                                        {
                                            aCell->properties.vAlign=caCenter;
                                        }
                                        else
                                        if (aComponent->ui->dataTableWidget->item(i, j)->textAlignment() & Qt::AlignBottom)
                                        {
                                            aCell->properties.vAlign=caBottom;
                                        }

                                        aCell->properties.cellMargin.setMargin(
                                                                               (int)(aComponent->typeColumns.at(j).topMargin*CM_TO_TWIPS),
                                                                               (int)(aComponent->typeColumns.at(j).leftMargin*CM_TO_TWIPS),
                                                                               (int)(aComponent->typeColumns.at(j).bottomMargin*CM_TO_TWIPS),
                                                                               (int)(aComponent->typeColumns.at(j).rightMargin*CM_TO_TWIPS)
                                                                              );

                                        if (aComponent->ui->dataTableWidget->item(i, j)->background().style()!=Qt::NoBrush)
                                        {
                                            aCell->properties.shading.pattern="pct-25";
                                            aCell->properties.shading.setColor(aComponent->ui->dataTableWidget->item(i, j)->background().color());
                                            aCell->properties.shading.backgroundColor=aCell->properties.shading.color;
                                            aCell->properties.shading.fillColor=aCell->properties.shading.color;
                                        }

                                        int invisibleColumns=0;

                                        for (int k=1; k<aCell->properties.columnSpan && headerI+k<aComponent->headerColumnWidths.length(); k++)
                                        {
                                            if (aComponent->typeColumns.at(j+k).visible)
                                            {
                                                aCell->properties.width+=aComponent->headerColumnWidths.at(headerI+k);
                                            }
                                            else
                                            {
                                                invisibleColumns++;
                                            }
                                        }

                                        aCell->properties.width=aCell->properties.width*PIXELS_TO_TWIPS;
                                        rowWidth+=aCell->properties.width;

                                        QString aText=variantToText(colValues.at(j));

                                        if (
                                            aComponent->typeColumns.at(j).column->type()==ctInteger
                                            &&
                                            !((IntegerColumn*)aComponent->typeColumns.at(j).column)->mIsAutoInc
                                            &&
                                            !((IntegerColumn*)aComponent->typeColumns.at(j).column)->mSplitRows
                                           )
                                        {
                                            aText=((IntegerColumn*)aComponent->typeColumns.at(j).column)->mPrefix+
                                                  aText+
                                                  ((IntegerColumn*)aComponent->typeColumns.at(j).column)->mPostfix;
                                        }

                                        do
                                        {
                                            WordXMLParagraph *aParagraph=aCell->addParagraph();
                                            WordXMLRun *aRun=aParagraph->addRun();

                                            if (aComponent->ui->dataTableWidget->item(i, j)->textAlignment() & Qt::AlignLeft)
                                            {
                                                aParagraph->properties.alignment=paLeft;
                                            }
                                            else
                                            if (aComponent->ui->dataTableWidget->item(i, j)->textAlignment() & Qt::AlignHCenter)
                                            {
                                                aParagraph->properties.alignment=paCenter;
                                            }
                                            else
                                            if (aComponent->ui->dataTableWidget->item(i, j)->textAlignment() & Qt::AlignRight)
                                            {
                                                aParagraph->properties.alignment=paRight;
                                            }

                                            aRun->properties.setFont(aComponent->ui->dataTableWidget->item(i, j)->font());
                                            aRun->properties.setColor(aComponent->ui->dataTableWidget->item(i, j)->foreground().color());

                                            QString aTextPart=aText.left(aText.indexOf("\n"));
                                            aText.remove(0, aTextPart.length()+1);

                                            aRun->addText(aTextPart);
                                        } while (aText!="");

                                        if (aCell->properties.columnSpan>1)
                                        {
                                            j+=aCell->properties.columnSpan-1;

                                            aCell->properties.columnSpan-=invisibleColumns;
                                        }
                                    }

                                    if (rowWidth>aTable->properties.width)
                                    {
                                        aTable->properties.width=rowWidth;
                                    }

                                    if (separations.length()>0)
                                    {
                                        QList<WordXMLTableRow *> aNewRows;
                                        aNewRows.append(aRow);

                                        for (int j=separations.length()-1; j>=0; j--)
                                        {
                                            bool ok;

                                            int aColumn=separations.at(j);
                                            int xmlColumn=separationsXMLColumn.at(j);
                                            int rowParts=(int)colValues.at(aColumn).toDouble(&ok);

                                            if (ok && rowParts>1)
                                            {
                                                QList<WordXMLTableRow *> aNewRowsTemp=aNewRows;

                                                for (int k=1; k<rowParts; k++)
                                                {
                                                    for (int g=0; g<aNewRowsTemp.length(); g++)
                                                    {
                                                        aRow=aTable->addRow();
                                                        *aRow=*aNewRowsTemp.at(g);
                                                        aNewRows.append(aRow);
                                                    }
                                                }

                                                for (int k=0; k<aNewRows.length(); k++)
                                                {
                                                    aRow=aNewRows.at(k);

                                                    WordXMLTableCell *aCell=aRow->getCell(xmlColumn);

                                                    if (aCell->count()>0)
                                                    {
                                                        if (aComponent->typeColumns.at(aColumn).visible)
                                                        {
                                                            while (aCell->count()>1)
                                                            {
                                                                delete aCell->get(1);
                                                                aCell->remove(1);
                                                            }

                                                            WordXMLParagraph *aParagraph=(WordXMLParagraph*)aCell->get(0);

                                                            while (aParagraph->count()>1)
                                                            {
                                                                delete aParagraph->get(1);
                                                                aParagraph->remove(1);
                                                            }

                                                            WordXMLRun *aRun=(WordXMLRun*)aParagraph->get(0);

                                                            aRun->clear();
                                                            aRun->addText(
                                                                          ((IntegerColumn*)aComponent->typeColumns.at(aColumn).column)->mPrefix+
                                                                          QString::number((k/aNewRowsTemp.length())+1)+
                                                                          ((IntegerColumn*)aComponent->typeColumns.at(aColumn).column)->mPostfix
                                                                         );
                                                        }

                                                        if (k==0)
                                                        {
                                                            for (int g=0; g<xmlColumn; g++)
                                                            {
                                                                aRow->getCell(g)->properties.vMergeType=mtRestart;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            for (int g=0; g<xmlColumn; g++)
                                                            {
                                                                aRow->getCell(g)->properties.vMergeType=mtContinue;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                if (aComponent->typeColumns.at(aColumn).visible)
                                                {
                                                    for (int k=0; k<aNewRows.length(); k++)
                                                    {
                                                        aRow=aNewRows.at(k);

                                                        WordXMLTableCell *aCell=aRow->getCell(xmlColumn);

                                                        if (aCell->count()>0)
                                                        {
                                                            while (aCell->count()>1)
                                                            {
                                                                delete aCell->get(1);
                                                                aCell->remove(1);
                                                            }

                                                            WordXMLParagraph *aParagraph=(WordXMLParagraph*)aCell->get(0);

                                                            while (aParagraph->count()>1)
                                                            {
                                                                delete aParagraph->get(1);
                                                                aParagraph->remove(1);
                                                            }

                                                            WordXMLRun *aRun=(WordXMLRun*)aParagraph->get(0);

                                                            aRun->clear();
                                                            aRun->addText("-");
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        ui->progressBar->setValue(ui->progressBar->value()+1);
                    }
                }
            }
        }

        word.saveToFile(aTempPath+"TempFile.xml");

        ui->progressBar->setValue(0);
    }
    catch(...)
    {
        addError("Не удалось сохранить документ Word");
        good=false;
    }

    if (good)
    {
        WordApp wordApp(this);
        wordApp.setVisible(true);

        WordDocument *document=0;

        try
        {
            if (aFileName.endsWith(".xml"))
            {
                copyFile(aTempPath+"TempFile.xml", aFileName);
                document=wordApp.documents()->open(aFileName);
            }
            else
            {
                document=wordApp.documents()->open(aTempPath+"TempFile.xml");

                if (QFile::exists(aFileName+".doc"))
                {
                    QFile::remove(aFileName+".doc");
                }

                if (QFile::exists(aFileName+".docx"))
                {
                    QFile::remove(aFileName+".docx");
                }

                document->saveAs(aFileName, wordApp.defaultFileFormat());
            }

            if (isFull && contentIndex>=0)
            {
                WordTable* table=document->sections()->item(contentIndex)->range()->tables()->item(0);

                QList<int> pageCounts;
                int sectionNumber=0;

                for (int i=0; i<ui->pagesTabWidget->count(); i++)
                {
                    PageFrame* aPage=(PageFrame*)ui->pagesTabWidget->widget(i);

                    if (aPage->ui->useCheckBox->isChecked())
                    {
                        pageCounts.append(document->sections()->item(sectionNumber)->range()->computeStatistics(wdStatisticPages));
                        sectionNumber++;
                    }
                }

                int curPage=1;

                for (int i=0; i<pageCounts.length(); i++)
                {
                    // Не знаю почему, но для первых страниц количество страниц больше на 1
                    // Возможно разрыв раздела тоже считается частью раздела
                    if (i<pageCounts.length()-1)
                    {
                        pageCounts[i]--;
                    }

                    table->cell(i+1, 2)->range()->InsertAfter(QString::number(pageCounts.at(i)));

                    if (pageCounts.at(i)==1)
                    {
                        table->cell(i+1, 3)->range()->InsertAfter(QString::number(curPage));
                    }
                    else
                    if (pageCounts.at(i)==2)
                    {
                        table->cell(i+1, 3)->range()->InsertAfter(QString::number(curPage)+", "+QString::number(curPage+1));
                    }
                    else
                    {
                        table->cell(i+1, 3)->range()->InsertAfter(QString::number(curPage)+"-"+QString::number(curPage+pageCounts.at(i)-1));
                    }

                    curPage+=pageCounts.at(i);
                }
            }

            document->save();
        }
        catch(...)
        {
            if (document)
            {
                document->close();
            }

            wordApp.quit();

            copyFile(aTempPath+"TempFile.xml", "C:/TempFile.xml");
            addError("Не удалось открыть созданный документ Word. Создана копия XML файла (C:\\TempFile.xml)");
        }
    }

    if (ui->logListWidget->count()>logsBefore)
    {
        if (ui->logListWidget->height()==0)
        {
            QList<int> aSizes;

            aSizes.append(ui->pagesTabWidget->height()*0.9);
            aSizes.append(ui->pagesTabWidget->height()*0.1);

            dividerSplitter->setSizes(aSizes);
        }

        QMessageBox::warning(this, protocolCreatorVersion, "Возникли проблемы при сохранении документа Word.\nПожалуйста, проверьте логи");
    }
}

void MainWindow::on_actionExport_triggered()
{
    exportToWord(false);
}

void MainWindow::on_actionGenerateWord_triggered()
{
    exportToWord(true);
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

void MainWindow::autoSaveTick()
{
    if (wantAutoSave)
    {
        autoSaveMode=true;

        try
        {
            on_actionSave_triggered();
        }
        catch(...)
        {
            QMessageBox::critical(this, protocolCreatorVersion, "Возникла проблема при автосохранении документа");
        }

        autoSaveMode=false;
    }
    else
    {
        QTimer::singleShot(10000, this, SLOT(autoSaveTick()));
    }
}

void MainWindow::on_pagesTabWidget_currentChanged(int index)
{
    if (ui->pagesTabWidget->widget(index) && ui->pagesTabWidget->widget(index)!=contentPage)
    {
        PageFrame *aPage=(PageFrame*)ui->pagesTabWidget->widget(index);

        for (int j=0; j<aPage->variables.length(); j++)
        {
            aPage->variables.at(j)->componentShown();
        }

        for (int j=0; j<aPage->components.length(); j++)
        {
            aPage->components.at(j)->componentShown();
        }
    }
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

void MainWindow::resetAutoSaveTimer()
{
    autoSaveTimer.stop();
    autoSaveTimer.start(300000);
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

    aSettings.setValue("Normal_State",true);
    aSettings.setValue("Geometry",saveGeometry());
    aSettings.setValue("Divider_state",dividerSplitter->saveState());

    aSettings.endGroup();
}

void MainWindow::loadState()
{
    QSettings aSettings(dir+"data/Config.ini",QSettings::IniFormat);
    aSettings.beginGroup("States");

    if (!aSettings.value("Normal_State", true).toBool())
    {
        QDir aDir(dir+"autosave/");
        QFileInfoList aFiles=aDir.entryInfoList();

        QDateTime aMaxDate(QDate(1900, 1, 1), QTime(0, 0, 0, 0));

        for (int i=0; i<aFiles.length(); i++)
        {
            if (!aFiles.at(i).isDir() && aMaxDate<aFiles.at(i).lastModified())
            {
                aMaxDate=aFiles.at(i).lastModified();
                mLastAutoSave=dir+"autosave/"+aFiles.at(i).fileName();
            }
        }
    }

    aSettings.setValue("Normal_State",false);

    restoreGeometry(aSettings.value("Geometry").toByteArray());
    dividerSplitter->restoreState(aSettings.value("Divider_state").toByteArray());

    aSettings.endGroup();
}
