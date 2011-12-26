#include "src/main/columneditdialog.h"

ColumnEditDialog::ColumnEditDialog(bool aEditMode, QTableWidget *aTableWidget, VariableExtendedListFrame *aTable, int aColumnIndex, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnEditDialog)
{
    ui->setupUi(this);

    mTableWidget=aTableWidget;
    mTable=aTable;
    mEditMode=aEditMode;
    mColumnIndex=aColumnIndex;
}

ColumnEditDialog::~ColumnEditDialog()
{
    applyChanges();

    delete ui;
}

void ColumnEditDialog::applyChanges()
{

}

void ColumnEditDialog::on_typeComboBox_currentIndexChanged(int index)
{
    ui->typeStackedWidget->setCurrentIndex(index);
}
