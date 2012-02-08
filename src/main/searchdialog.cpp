#include "src/other/global.h"

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    QFile file(dir+"data/searches.txt");

    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
        QTextStream aStream(&file);
        aStream.setCodec("UTF-8");

        while (!aStream.atEnd())
        {
            try
            {
                QString st=aStream.readLine();
                ui->findComboBox->insertItem(0,st);
            }
            catch (...)
            {}
        }
    }

    ui->findComboBox->setCurrentIndex(ui->findComboBox->findText(lastSearch));
    ui->findComboBox->setEditText(lastSearch);

    QFile file2(dir+"data/replaces.txt");

    if (file2.exists())
    {
        file2.open(QIODevice::ReadOnly);
        QTextStream aStream(&file2);
        aStream.setCodec("UTF-8");

        while (!aStream.atEnd())
        {
            try
            {
                QString st=aStream.readLine();
                ui->replaceComboBox->insertItem(0,st);
            }
            catch (...)
            {}
        }
    }

    ui->replaceComboBox->setCurrentIndex(ui->replaceComboBox->findText(lastReplace));
    ui->replaceComboBox->setEditText(lastReplace);

    ui->findComboBox->setFocus();
    ui->findComboBox->lineEdit()->selectAll();
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::closeEvent(QCloseEvent *event)
{
    deleteLater();
    searchDialog=0;

    event->accept();
}

void SearchDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Escape)
    {
        deleteLater();
        searchDialog=0;
    }

    QDialog::keyPressEvent(event);
}

void SearchDialog::on_replaceGroupBox_toggled(bool checked)
{
    ui->replaceButton->setEnabled(checked);
    ui->replaceAllButton->setEnabled(checked);
}

void SearchDialog::on_replaceButton_clicked()
{
    find(0);
}

void SearchDialog::on_replaceAllButton_clicked()
{
    find(1);
}

void SearchDialog::on_findButton_clicked()
{
    find(2);
}

void SearchDialog::find(int mode)
{
    lastSearch=ui->findComboBox->currentText();
    lastReplace=ui->replaceComboBox->currentText();

    mainWindow->ui->actionFindNext->setEnabled(lastSearch!="");
    mainWindow->ui->actionFindPrev->setEnabled(lastSearch!="");

    if (lastSearch!="")
    {
        QDir(dir).mkpath(dir+"data");
        QFile file(dir+"data/searches.txt");
        file.open(QIODevice::WriteOnly);

        int index=ui->findComboBox->findText(lastSearch);

        if (index>=0)
        {
            ui->findComboBox->removeItem(index);
        }

        ui->findComboBox->insertItem(0, lastSearch);
        ui->findComboBox->setCurrentIndex(0);

        QTextStream aStream(&file);
        aStream.setCodec("UTF-8");

        for (int i=0; i<ui->findComboBox->count(); i++)
        {
            if (i>0)
            {
                aStream<<"\r\n";
            }

            aStream<<ui->findComboBox->itemText(ui->findComboBox->count()-1-i);
        }

        if (ui->replaceGroupBox->isChecked())
        {
            QFile file2(dir+"data/replaces.txt");
            file2.open(QIODevice::WriteOnly);

            index=ui->replaceComboBox->findText(lastReplace);

            if (index>=0)
            {
                ui->replaceComboBox->removeItem(index);
            }

            ui->replaceComboBox->insertItem(0, lastReplace);
            ui->replaceComboBox->setCurrentIndex(0);

            QTextStream aStream2(&file2);
            aStream2.setCodec("UTF-8");

            for (int i=0; i<ui->replaceComboBox->count(); i++)
            {
                if (i>0)
                {
                    aStream2<<"\r\n";
                }

                aStream2<<ui->replaceComboBox->itemText(ui->replaceComboBox->count()-1-i);
            }
        }

        isReplace=false;
        isFindAll=false;

        switch (mode)
        {
            case 0:
            {
                isReplace=true;
                mainWindow->on_actionFindNext_triggered();
                isReplace=false;
            }
            break;
            case 1:
            {
                isReplace=true;
                isFindAll=true;
                mainWindow->on_actionFindNext_triggered();
                isReplace=false;
                isFindAll=false;
            }
            break;
            case 2:
            {
                mainWindow->on_actionFindNext_triggered();
            }
            break;
        }
    }
}
