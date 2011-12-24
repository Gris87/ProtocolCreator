#include "src/main/columneditdialog.h"

ColumnEditDialog::ColumnEditDialog(bool editMode, QTableWidget *aTableWidget, VariableExtendedListFrame *aTable, int columnIndex, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnEditDialog)
{
    ui->setupUi(this);

    mTableWidget=aTableWidget;
    mTable=aTable;
}

ColumnEditDialog::~ColumnEditDialog()
{
    delete ui;
}
