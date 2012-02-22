#include "src/other/global.h"

ComponentSelectionDialog::ComponentSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComponentSelectionDialog)
{
    ui->setupUi(this);



    dividerSplitter = new QSplitter(Qt::Horizontal, this);

    ui->dividerLayout->removeWidget(ui->pagesListWidget);
    ui->dividerLayout->removeWidget(ui->variablesListWidget);

    dividerSplitter->addWidget(ui->pagesListWidget);
    dividerSplitter->addWidget(ui->variablesListWidget);

    ui->dividerLayout->addWidget(dividerSplitter);

    QList<int> aSizes;

    aSizes.append(240);
    aSizes.append(240);

    dividerSplitter->setSizes(aSizes);

    dividerSplitter->setCollapsible(0, false);
    dividerSplitter->setCollapsible(1, false);



    ui->pagesListWidget->addItem("Global");

    for (int i=0; i<mainWindow->ui->pagesTabWidget->count(); i++)
    {
        ui->pagesListWidget->addItem(((PageFrame*)mainWindow->ui->pagesTabWidget->widget(i))->ui->varNameEdit->text());
    }

    ui->pagesListWidget->setCurrentRow(globalDialog->isVisible()? 0 : mainWindow->ui->pagesTabWidget->currentIndex()+1);
}

ComponentSelectionDialog::~ComponentSelectionDialog()
{
    delete ui;
}

void ComponentSelectionDialog::on_pagesListWidget_currentRowChanged(int currentRow)
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
            if (
                !globalDialog->variables.at(i)->inherits("VariableExtendedListFrame")
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
                !aPage->variables.at(i)->inherits("VariableExtendedListFrame")
               )
            {
                ui->variablesListWidget->addItem(aPage->variables.at(i)->variableName());
            }
        }
    }
}

void ComponentSelectionDialog::on_variablesListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    on_okButton_clicked();
}

void ComponentSelectionDialog::on_okButton_clicked()
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

    mResult=ui->pagesListWidget->currentItem()->text()+"."+ui->variablesListWidget->currentItem()->text();

    accept();
}

void ComponentSelectionDialog::on_cancelButton_clicked()
{
    reject();
}
