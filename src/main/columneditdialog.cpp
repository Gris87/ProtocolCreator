#include "src/main/columneditdialog.h"

ColumnEditDialog::ColumnEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnEditDialog)
{
    ui->setupUi(this);
}

ColumnEditDialog::~ColumnEditDialog()
{
    delete ui;
}
