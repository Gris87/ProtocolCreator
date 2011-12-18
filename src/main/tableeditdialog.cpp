#include "src/other/global.h"

TableEditDialog::TableEditDialog(VariableExtendedListFrame *aTable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableEditDialog)
{
    ui->setupUi(this);

    mTable=aTable;

    updateAdmin();
}

TableEditDialog::~TableEditDialog()
{
    delete ui;
}

void TableEditDialog::updateAdmin()
{

}
