#include "src/other/global.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    ui->progressBar->setValue(0);

    connect(ui->pagesTabWidget->tabBar(), SIGNAL(tabMoved(int,int)), this, SLOT(pageMoved(int,int)));

    loadState();

    on_actionNew_triggered();
}

MainWindow::~MainWindow()
{
    saveState();

    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    currentName="";
    isAdmin=true;
    docPass="";
    adminPass="";

    updateHeader();

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

        //DecryptStream(aArray, "Thunderbolt");

        QDataStream aStream(&aArray, QIODevice::ReadOnly);

        QString aMagicWord;
        aStream >> aMagicWord;

        if (aMagicWord!="ProtocolCreator")
        {
            QMessageBox::warning(this, "Openning file", "Incorrect format");
            return;
        }

        aStream >> aMagicWord;

        if (aMagicWord!="DocumentPassword")
        {
            QMessageBox::warning(this, "Openning file", "Incorrect format");
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

        aStream << QString("Pages");

        aStream << QString("Page");
        contentPage->saveToStream(aStream);

        int pageIndex=ui->pagesTabWidget->indexOf(contentPage);

        ui->progressBar->setMaximum(ui->pagesTabWidget->count());

        for (int i=0; i<ui->pagesTabWidget->count(); i++)
        {
            ui->progressBar->setValue(i);

            if (i!=pageIndex)
            {
                aStream << QString("Page");
                ((PageFrame*)ui->pagesTabWidget->widget(i))->saveToStream(aStream);
            }
        }

        ui->progressBar->setValue(0);

        aStream << QString("Stop");

        aStream << QString("ContentIndex");
        aStream << pageIndex;

        //EncryptStream(aArray, "Thunderbolt");

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
    PasswordDialog dialog(this);
    dialog.ui->titleLabel->setText("Введите пароль на документ");

    if (dialog.exec())
    {
        docPass=EncryptString(dialog.ui->passwordEdit->text(), "Earthquake");
    }
}

void MainWindow::on_actionSetAdminPass_triggered()
{
    PasswordDialog dialog(this);
    dialog.ui->titleLabel->setText("Введите пароль администратора");

    if (dialog.exec())
    {
        adminPass=EncryptString(dialog.ui->passwordEdit->text(), "Superman");
    }
}

void MainWindow::on_actionAddPage_triggered()
{
    if (isAdmin)
    {
        addPage("Новый раздел", "Section"+QString::number(ui->pagesTabWidget->count()+1));
    }
}

void MainWindow::pageMoved(int from, int to)
{
    QWidget* aWidget=contentPage->ui->pageLayout->itemAt(from)->widget();

    contentPage->ui->pageLayout->removeWidget(aWidget);
    contentPage->ui->pageLayout->insertWidget(to, aWidget);
}

void MainWindow::page_nameChanged(PageFrame *parentPage)
{
    int pageIndex=ui->pagesTabWidget->indexOf(parentPage);

    if (pageIndex<0)
    {
        return;
    }

    ui->pagesTabWidget->setTabText(pageIndex, parentPage->ui->nameEdit->text());
    ((KnownCheckBox*)contentPage->ui->pageLayout->itemAt(pageIndex)->widget())->setText(parentPage->ui->nameEdit->text());
}

void MainWindow::page_useToggled(PageFrame *parentPage)
{
    int pageIndex=ui->pagesTabWidget->indexOf(parentPage);

    if (pageIndex<0)
    {
        return;
    }

    ((KnownCheckBox*)contentPage->ui->pageLayout->itemAt(pageIndex)->widget())->setChecked(parentPage->ui->useCheckBox->isChecked());
}

void MainWindow::contentCheckBoxToggled(KnownCheckBox *aCheckBox, bool checked)
{
    int pageIndex=contentPage->ui->pageLayout->indexOf(aCheckBox);

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
            QWidget* aWidget=contentPage->ui->pageLayout->itemAt(index)->widget();
            contentPage->ui->pageLayout->removeWidget(aWidget);
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
    contentPage->ui->pageLayout->addWidget(aCheckBox);

    aNewPage->ui->nameEdit->setText(aName);
    aNewPage->ui->varNameEdit->setText(aVarName);

    connect(aCheckBox, SIGNAL(checked(KnownCheckBox*,bool)), this, SLOT(contentCheckBoxToggled(KnownCheckBox*,bool)));
    connect(aNewPage, SIGNAL(nameChanged(PageFrame*)), this, SLOT(page_nameChanged(PageFrame*)));
    connect(aNewPage, SIGNAL(useToggled(PageFrame*)), this, SLOT(page_useToggled(PageFrame*)));
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
    for (int i=0; i<ui->pagesTabWidget->count(); i++)
    {
        ((PageFrame*)ui->pagesTabWidget->widget(i))->updateAdmin();
    }

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
    ui->actionAddPage->setVisible(isAdmin);
}

void MainWindow::saveState()
{
    QDir(dir).mkpath(dir+"data");
    QFile::remove(dir+"data/Config.ini");
    QSettings aSettings(dir+"data/Config.ini",QSettings::IniFormat);

    aSettings.beginGroup("States");

    aSettings.setValue("Geometry",saveGeometry());

    aSettings.endGroup();
}

void MainWindow::loadState()
{
    QSettings aSettings(dir+"data/Config.ini",QSettings::IniFormat);
    aSettings.beginGroup("States");

    restoreGeometry(aSettings.value("Geometry").toByteArray());

    aSettings.endGroup();
}
