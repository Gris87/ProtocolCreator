#include "src/other/global.h"

PageSelectionDialog::PageSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PageSelectionDialog)
{
    ui->setupUi(this);

    ui->pagesListWidget->addItem("Глобальные переменные");

    for (int i=0; i<mainWindow->ui->pagesTabWidget->count(); i++)
    {
        ui->pagesListWidget->addItem(((PageFrame*)mainWindow->ui->pagesTabWidget->widget(i))->ui->nameEdit->text());
    }

    ui->pagesListWidget->setCurrentRow(globalDialog->isVisible()? 0 : mainWindow->ui->pagesTabWidget->currentIndex()+1);
}

PageSelectionDialog::~PageSelectionDialog()
{
    delete ui;
}

void PageSelectionDialog::on_cancelButton_clicked()
{
    reject();
}

void PageSelectionDialog::on_okButton_clicked()
{
    if (ui->pagesListWidget->currentRow()<0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите раздел");
        return;
    }

    accept();
}

void PageSelectionDialog::on_pagesListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    on_okButton_clicked();
}
