#include "src/other/global.h"

ListSelectionDialog::ListSelectionDialog(bool aExtListOnly, QString aBaseList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListSelectionDialog)
{
    ui->setupUi(this);

    mExtListOnly=aExtListOnly;



    dividerSplitter = new QSplitter(Qt::Horizontal, this);

    ui->dividerLayout->removeWidget(ui->pagesListWidget);
    ui->dividerLayout->removeWidget(ui->variablesListWidget);
    ui->dividerLayout->removeWidget(ui->columnListWidget);

    dividerSplitter->addWidget(ui->pagesListWidget);
    dividerSplitter->addWidget(ui->variablesListWidget);

    if (!mExtListOnly)
    {
        dividerSplitter->addWidget(ui->columnListWidget);
    }

    ui->dividerLayout->addWidget(dividerSplitter);

    QList<int> aSizes;

    if (mExtListOnly)
    {
        aSizes.append(390);
        aSizes.append(250);
    }
    else
    {
        aSizes.append(240);
        aSizes.append(200);
        aSizes.append(200);
    }

    dividerSplitter->setSizes(aSizes);

    dividerSplitter->setCollapsible(0, false);
    dividerSplitter->setCollapsible(1, false);

    if (!mExtListOnly)
    {
        dividerSplitter->setCollapsible(2, false);
    }



    ui->pagesListWidget->addItem("Global");

    for (int i=0; i<mainWindow->ui->pagesTabWidget->count(); i++)
    {
        ui->pagesListWidget->addItem(((PageFrame*)mainWindow->ui->pagesTabWidget->widget(i))->ui->varNameEdit->text());
    }

    ui->pagesListWidget->setCurrentRow(globalDialog->isVisible()? 0 : mainWindow->ui->pagesTabWidget->currentIndex()+1);

    if (aBaseList!="")
    {
        int index=aBaseList.indexOf(".");
        QString aSectionName=aBaseList.left(index);
        QString aVarName=aBaseList.mid(index+1);

        index=-1;

        for (int i=0; i<ui->pagesListWidget->count(); i++)
        {
            if (ui->pagesListWidget->item(i)->text()==aSectionName)
            {
                index=i;
                break;
            }
        }

        if (index>=0)
        {
            ui->pagesListWidget->setCurrentRow(index);

            QString aColumn;

            index=aVarName.indexOf("[");

            if (index>=0)
            {
                aColumn=aVarName.mid(index+1);
                aColumn.remove(aColumn.length()-1, 1);

                aVarName=aVarName.left(index);
            }

            index=-1;

            for (int i=0; i<ui->variablesListWidget->count(); i++)
            {
                if (ui->variablesListWidget->item(i)->text()==aVarName)
                {
                    index=i;
                    break;
                }
            }

            if (index>=0)
            {
                ui->variablesListWidget->setCurrentRow(index);

                if (aColumn!="")
                {
                    bool ok;
                    int aColumnID=aColumn.toInt(&ok);

                    if (ok && aColumnID>=1 && aColumnID<=ui->columnListWidget->count())
                    {
                        ui->columnListWidget->setCurrentRow(aColumnID-1);
                    }
                }
            }
        }
    }
}

ListSelectionDialog::~ListSelectionDialog()
{
    delete ui;
}

void ListSelectionDialog::on_pagesListWidget_currentRowChanged(int currentRow)
{
    ui->variablesListWidget->clear();
    ui->columnListWidget->clear();

    if (currentRow<0)
    {
        return;
    }

    if (currentRow==0)
    {
        for (int i=0; i<globalDialog->variables.length(); i++)
        {
            if (
                (
                 !mExtListOnly
                 &&
                 globalDialog->variables.at(i)->inherits("VariableListFrame")
                )
                ||
                globalDialog->variables.at(i)->inherits("VariableExtendedListFrame")
               )
            {
                ui->variablesListWidget->addItem(globalDialog->variables.at(i)->variableName());
            }
        }
    }
    else
    {
        PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(currentRow-1);

        for (int i=0; i<aPage->variables.length(); i++)
        {
            if (
                (
                 !mExtListOnly
                 &&
                 aPage->variables.at(i)->inherits("VariableListFrame")
                )
                ||
                aPage->variables.at(i)->inherits("VariableExtendedListFrame")
               )
            {
                ui->variablesListWidget->addItem(aPage->variables.at(i)->variableName());
            }
        }

        for (int i=0; i<aPage->components.length(); i++)
        {
            if (aPage->components.at(i)->inherits("VariableExtendedListFrame"))
            {
                ui->variablesListWidget->addItem(aPage->components.at(i)->variableName());
            }
        }
    }
}

void ListSelectionDialog::on_variablesListWidget_currentRowChanged(int currentRow)
{
    if (mExtListOnly)
    {
        return;
    }

    ui->columnListWidget->clear();

    if (currentRow<0)
    {
        return;
    }

    QString varName=ui->variablesListWidget->item(currentRow)->text();
    VariableExtendedListFrame *aFrame=0;

    if (ui->pagesListWidget->currentRow()==0)
    {
        for (int i=0; i<globalDialog->variables.length(); i++)
        {
            if (
                globalDialog->variables.at(i)->variableName()==varName
                &&
                globalDialog->variables.at(i)->inherits("VariableExtendedListFrame")
               )
            {
                aFrame=(VariableExtendedListFrame*)globalDialog->variables[i];
                break;
            }
        }
    }
    else
    {
        PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(ui->pagesListWidget->currentRow()-1);

        for (int i=0; i<aPage->variables.length(); i++)
        {
            if (
                aPage->variables.at(i)->variableName()==varName
                &&
                aPage->variables.at(i)->inherits("VariableExtendedListFrame")
               )
            {
                aFrame=(VariableExtendedListFrame*)aPage->variables[i];
                break;
            }
        }

        if (aFrame==0)
        {
            for (int i=0; i<aPage->components.length(); i++)
            {
                if (
                    aPage->components.at(i)->variableName()==varName
                    &&
                    aPage->components.at(i)->inherits("VariableExtendedListFrame")
                   )
                {
                    aFrame=(VariableExtendedListFrame*)aPage->components[i];
                    break;
                }
            }
        }
    }

    if (aFrame)
    {
        for (int i=0; i<aFrame->typeColumns.length(); i++)
        {
            ui->columnListWidget->addItem("Столбец_"+QString::number(i+1)+" "+aFrame->typeColumns.at(i).name);
        }
    }
}

void ListSelectionDialog::on_okButton_clicked()
{
    if (ui->pagesListWidget->currentRow()<0)
    {
        QMessageBox::information(this, "Выбор списка", "Выберите раздел");
        return;
    }

    if (ui->variablesListWidget->currentRow()<0)
    {
        QMessageBox::information(this, "Выбор списка", "Выберите переменную");
        return;
    }

    if (!mExtListOnly)
    {
        QString varName=ui->variablesListWidget->currentItem()->text();
        VariableExtendedListFrame *aFrame=0;

        if (ui->pagesListWidget->currentRow()==0)
        {
            for (int i=0; i<globalDialog->variables.length(); i++)
            {
                if (
                    globalDialog->variables.at(i)->variableName()==varName
                    &&
                    globalDialog->variables.at(i)->inherits("VariableExtendedListFrame")
                   )
                {
                    aFrame=(VariableExtendedListFrame*)globalDialog->variables[i];
                    break;
                }
            }
        }
        else
        {
            PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(ui->pagesListWidget->currentRow()-1);

            for (int i=0; i<aPage->variables.length(); i++)
            {
                if (
                    aPage->variables.at(i)->variableName()==varName
                    &&
                    aPage->variables.at(i)->inherits("VariableExtendedListFrame")
                   )
                {
                    aFrame=(VariableExtendedListFrame*)aPage->variables[i];
                    break;
                }
            }

            if (aFrame==0)
            {
                for (int i=0; i<aPage->components.length(); i++)
                {
                    if (
                        aPage->components.at(i)->variableName()==varName
                        &&
                        aPage->components.at(i)->inherits("VariableExtendedListFrame")
                       )
                    {
                        aFrame=(VariableExtendedListFrame*)aPage->components[i];
                        break;
                    }
                }
            }
        }

        if (aFrame)
        {
            if (ui->columnListWidget->currentRow()<0)
            {
                QMessageBox::information(this, "Выбор списка", "Выберите столбец");
                return;
            }
        }
    }

    mResult=ui->pagesListWidget->currentItem()->text()+"."+ui->variablesListWidget->currentItem()->text();

    if (ui->columnListWidget->currentRow()>=0)
    {
        mResult.append("[");
        mResult.append(QString::number(ui->columnListWidget->currentRow()+1));
        mResult.append("]");
    }

    accept();
}

void ListSelectionDialog::on_cancelButton_clicked()
{
    reject();
}
