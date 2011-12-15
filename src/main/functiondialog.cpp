#include "src/other/global.h"

FunctionDialog::FunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FunctionDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);



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

    dividerSplitter2->addWidget(ui->functionWidget);
    dividerSplitter2->addWidget(ui->pagesListWidget);
    dividerSplitter2->addWidget(ui->variablesListWidget);

    ui->dividerLayout_2->addWidget(dividerSplitter2);

    aSizes.clear();

    aSizes.append(500);
    aSizes.append(150);
    aSizes.append(150);

    dividerSplitter2->setSizes(aSizes);

    dividerSplitter2->setCollapsible(0, false);
    dividerSplitter2->setCollapsible(1, false);
    dividerSplitter2->setCollapsible(2, false);



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

void FunctionDialog::on_variablesListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->expressionEdit->insert(ui->pagesListWidget->currentItem()->text()+"."+item->text());
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
