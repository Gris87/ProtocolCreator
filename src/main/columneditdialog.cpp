#include "src/other/global.h"

ColumnEditDialog::ColumnEditDialog(bool aEditMode, QTableWidget *aTableWidget, VariableExtendedListFrame *aTable, int aColumnIndex, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnEditDialog)
{
    ui->setupUi(this);

    mTableWidget=aTableWidget;
    mTable=aTable;
    mEditMode=aEditMode;
    mColumnIndex=aColumnIndex;

    if (mEditMode)
    {

    }
}

ColumnEditDialog::~ColumnEditDialog()
{
    applyChanges();

    delete ui;
}

void ColumnEditDialog::applyChanges()
{
    if (mEditMode)
    {

    }
    else
    {
        STableColumn aColumn;

        aColumn.name=ui->nameEdit->text();

        switch (ui->typeComboBox->currentIndex())
        {
            case 0:
            {
                IntegerColumn *aTypeColumn=new IntegerColumn();

                aTypeColumn->mDefaultValue=ui->integerDefaultSpinBox->value();
                aTypeColumn->mDecimals=(int)ui->integerNumberSpinBox->value();
                aTypeColumn->mIsAutoInc=ui->integerAutoIncrementCheckBox->isChecked();
                aTypeColumn->mPrefix=ui->integerPrefixEdit->text();
                aTypeColumn->mPostfix=ui->integerPostfixEdit->text();

                aColumn.column=aTypeColumn;
            }
            break;
            case 1:
            {
                StringColumn *aTypeColumn=new StringColumn();

                aTypeColumn->mDefaultValue=ui->stringDefaultEdit->text();

                aColumn.column=aTypeColumn;
            }
            break;
            case 2:
            {
                BoolColumn *aTypeColumn=new BoolColumn();

                aTypeColumn->mDefaultValue=ui->boolDefaultCheckBox->isChecked();

                aColumn.column=aTypeColumn;
            }
            break;
            case 3:
            {
                DateColumn *aTypeColumn=new DateColumn();

                aTypeColumn->mDefaultValue=ui->dateDefaultEdit->date();

                aColumn.column=aTypeColumn;
            }
            break;
            case 4:
            {
                TimeColumn *aTypeColumn=new TimeColumn();

                aTypeColumn->mDefaultValue=ui->timeDefaultEdit->time();

                aColumn.column=aTypeColumn;
            }
            break;
            default:
            {
                throw "Unknown column type";
            }
            break;
        }

        aColumn.leftOffset=ui->leftOffsetSpinBox->value();
        aColumn.rightOffset=ui->rightOffsetSpinBox->value();
        aColumn.totalOffset=ui->totalOffsetSpinBox->value();

        mTable->typeColumns.insert(mColumnIndex, aColumn);

        mTableWidget->insertColumn(mColumnIndex);
        mTableWidget->setHorizontalHeaderItem(mColumnIndex, new QTableWidgetItem(aColumn.name));
        mTableWidget->setItem(0, mColumnIndex, new QTableWidgetItem(aColumn.column->typeDescription()));

        mTable->ui->dataTableWidget->insertColumn(mColumnIndex);
        mTable->ui->dataTableWidget->setHorizontalHeaderItem(mColumnIndex, new QTableWidgetItem(aColumn.name));
    }
}

void ColumnEditDialog::on_typeComboBox_currentIndexChanged(int index)
{
    ui->typeStackedWidget->setCurrentIndex(index);
}

void ColumnEditDialog::on_integerNumberSpinBox_valueChanged(double value)
{
    ui->integerDefaultSpinBox->setDecimals((int)value);
}
