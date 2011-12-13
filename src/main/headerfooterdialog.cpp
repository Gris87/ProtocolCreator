#include "src/other/global.h"

HeaderFooterDialog::HeaderFooterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HeaderFooterDialog)
{
    ui->setupUi(this);
}

HeaderFooterDialog::~HeaderFooterDialog()
{
    delete ui;
}
