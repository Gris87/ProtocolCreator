#include "columnsizedialog.h"

ColumnSizeDialog::ColumnSizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnSizeDialog)
{
    ui->setupUi(this);
}

ColumnSizeDialog::~ColumnSizeDialog()
{
    delete ui;
}

void ColumnSizeDialog::on_okButton_clicked()
{
    accept();
}

void ColumnSizeDialog::on_cancelButton_clicked()
{
    reject();
}
