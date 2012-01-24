#include "src/other/global.h"

FunctionDialog::FunctionDialog(VariableExtendedListFrame *aExtList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FunctionDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    mExtList=aExtList;



    dividerSplitter = new QSplitter(Qt::Vertical, this);

    ui->dividerLayout->removeWidget(ui->functionsListWidget);
    ui->dividerLayout->removeWidget(ui->descriptionEdit);

    dividerSplitter->addWidget(ui->functionsListWidget);
    dividerSplitter->addWidget(ui->descriptionEdit);

    ui->dividerLayout->addWidget(dividerSplitter);

    QList<int> aSizes;

    aSizes.append(400);
    aSizes.append(100);

    dividerSplitter->setSizes(aSizes);

    dividerSplitter->setCollapsible(0, false);
    dividerSplitter->setCollapsible(1, false);



    dividerSplitter2 = new QSplitter(Qt::Horizontal, this);

    ui->dividerLayout_2->removeWidget(ui->functionWidget);
    ui->dividerLayout_2->removeWidget(ui->pagesListWidget);
    ui->dividerLayout_2->removeWidget(ui->variablesListWidget);
    ui->dividerLayout_2->removeWidget(ui->columnListWidget);

    dividerSplitter2->addWidget(ui->functionWidget);
    dividerSplitter2->addWidget(ui->pagesListWidget);
    dividerSplitter2->addWidget(ui->variablesListWidget);
    dividerSplitter2->addWidget(ui->columnListWidget);

    ui->dividerLayout_2->addWidget(dividerSplitter2);

    aSizes.clear();

    aSizes.append(400);
    aSizes.append(200);
    aSizes.append(200);
    aSizes.append(200);

    dividerSplitter2->setSizes(aSizes);

    dividerSplitter2->setCollapsible(0, false);
    dividerSplitter2->setCollapsible(1, false);
    dividerSplitter2->setCollapsible(2, false);
    dividerSplitter2->setCollapsible(3, false);



    ui->pagesListWidget->addItem("Global");

    for (int i=0; i<mainWindow->ui->pagesTabWidget->count(); i++)
    {
        ui->pagesListWidget->addItem(((PageFrame*)mainWindow->ui->pagesTabWidget->widget(i))->ui->varNameEdit->text());
    }

    ui->pagesListWidget->setCurrentRow(mainWindow->ui->pagesTabWidget->currentIndex()+1);

    ui->functionsListWidget->setCurrentRow(0);
}

FunctionDialog::~FunctionDialog()
{
    delete ui;
}

void FunctionDialog::on_functionsListWidget_currentRowChanged(int currentRow)
{
    if (currentRow<0)
    {
        ui->descriptionEdit->clear();
        return;
    }

    ui->descriptionEdit->setHtml(ui->functionsListWidget->item(currentRow)->toolTip());
}

void FunctionDialog::on_functionsListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString aDescription=ui->descriptionEdit->toPlainText();

    aDescription=aDescription.left(aDescription.indexOf("\n"));
    aDescription=aDescription.left(aDescription.lastIndexOf(":"));

    if (aDescription.endsWith(")"))
    {
        int cur=aDescription.length()-2;

        while (cur>=0 && aDescription.at(cur)!='(')
        {
            if (aDescription.at(cur)!=';')
            {
                aDescription.remove(cur, 1);
            }

            cur--;
        }

        ui->expressionEdit->insert(aDescription);

        ui->expressionEdit->setCursorPosition(ui->expressionEdit->cursorPosition()-aDescription.length()+aDescription.lastIndexOf("(")+1);
    }
    else
    {
        ui->expressionEdit->insert(aDescription+"()");
    }

    ui->expressionEdit->setFocus();
}

void FunctionDialog::on_pagesListWidget_currentRowChanged(int currentRow)
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
            ui->variablesListWidget->addItem(globalDialog->variables.at(i)->variableName());
        }
    }
    else
    {
        PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(currentRow-1);

        for (int i=0; i<aPage->variables.length(); i++)
        {
            ui->variablesListWidget->addItem(aPage->variables.at(i)->variableName());
        }

        for (int i=0; i<aPage->components.length(); i++)
        {
            ui->variablesListWidget->addItem(aPage->components.at(i)->variableName());
        }
    }
}

void FunctionDialog::on_variablesListWidget_currentRowChanged(int currentRow)
{
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

void FunctionDialog::on_variablesListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->expressionEdit->insert(ui->pagesListWidget->currentItem()->text()+"."+item->text());
    ui->expressionEdit->setFocus();
}

void FunctionDialog::on_columnListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (
        mExtList
        &&
        ui->pagesListWidget->currentRow()==mainWindow->ui->pagesTabWidget->currentIndex()+1
        &&
        ui->variablesListWidget->currentItem()->text()==mExtList->variableName()
       )
    {
        ui->expressionEdit->insert("["+QString::number(ui->columnListWidget->row(item)+1)+"]");
    }
    else
    {
        ui->expressionEdit->insert(ui->pagesListWidget->currentItem()->text()+"."+ui->variablesListWidget->currentItem()->text()+"["+QString::number(ui->columnListWidget->row(item)+1)+"]");
    }

    ui->expressionEdit->setFocus();
}

void FunctionDialog::on_okButton_clicked()
{
    accept();
}

void FunctionDialog::on_cancelButton_clicked()
{
    reject();
}
