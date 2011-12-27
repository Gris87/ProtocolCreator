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
        startEditing();
    }
    else
    {
        ui->leftButton->setVisible(false);
        ui->rightButton->setVisible(false);
    }
}

ColumnEditDialog::~ColumnEditDialog()
{
    applyChanges();

    delete ui;
}

void ColumnEditDialog::startEditing()
{
    ui->leftButton->setEnabled(mColumnIndex>0);
    ui->rightButton->setEnabled(mColumnIndex<mTable->typeColumns.length()-1);

    STableColumn *aColumn=&mTable->typeColumns[mColumnIndex];

    ui->nameEdit->setText(aColumn->name);

    switch (aColumn->column->type())
    {
        case ctInteger:
        {
            ui->typeComboBox->setCurrentIndex(0);

            ui->integerDefaultSpinBox->setValue         (((IntegerColumn*)aColumn->column)->mDefaultValue);
            ui->integerNumberSpinBox->setValue          (((IntegerColumn*)aColumn->column)->mDecimals);
            ui->integerAutoIncrementCheckBox->setChecked(((IntegerColumn*)aColumn->column)->mIsAutoInc);
            ui->integerPrefixEdit->setText              (((IntegerColumn*)aColumn->column)->mPrefix);
            ui->integerPostfixEdit->setText             (((IntegerColumn*)aColumn->column)->mPostfix);
        }
        break;
        case ctString:
        {
            ui->typeComboBox->setCurrentIndex(1);

            ui->stringDefaultEdit->setText(((StringColumn*)aColumn->column)->mDefaultValue);
        }
        break;
        case ctBool:
        {
            ui->typeComboBox->setCurrentIndex(2);

            ui->boolDefaultCheckBox->setChecked(((BoolColumn*)aColumn->column)->mDefaultValue);
        }
        break;
        case ctDate:
        {
            ui->typeComboBox->setCurrentIndex(3);

            ui->dateDefaultEdit->setDate(((DateColumn*)aColumn->column)->mDefaultValue);
        }
        break;
        case ctTime:
        {
            ui->typeComboBox->setCurrentIndex(4);

            ui->timeDefaultEdit->setTime(((TimeColumn*)aColumn->column)->mDefaultValue);
        }
        break;
        default:
        {
            throw "Unknown column type";
        }
        break;
    }

    ui->leftOffsetSpinBox->setValue(aColumn->leftOffset);
    ui->rightOffsetSpinBox->setValue(aColumn->rightOffset);
    ui->totalOffsetSpinBox->setValue(aColumn->totalOffset);
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

void ColumnEditDialog::on_leftButton_clicked()
{
    applyChanges();

    mColumnIndex--;
    mTableWidget->setCurrentCell(0, mColumnIndex);

    startEditing();
}

void ColumnEditDialog::on_rightButton_clicked()
{
    applyChanges();

    mColumnIndex++;
    mTableWidget->setCurrentCell(0, mColumnIndex);

    startEditing();
}
