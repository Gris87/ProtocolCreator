#include "src/other/global.h"

ColumnEditDialog::ColumnEditDialog(bool aEditMode, UnitedTable *aTableWidget, VariableExtendedListFrame *aTable, int aColumnIndex, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnEditDialog)
{
    ui->setupUi(this);

    mTableWidget=aTableWidget;
    mTable=aTable;
    mEditMode=aEditMode;
    mColumnIndex=aColumnIndex;

    ui->listLinkPagesListWidget->addItem("Global");

    for (int i=0; i<mainWindow->ui->pagesTabWidget->count(); i++)
    {
        ui->listLinkPagesListWidget->addItem(((PageFrame*)mainWindow->ui->pagesTabWidget->widget(i))->ui->varNameEdit->text());
    }

    ui->listLinkPagesListWidget->setCurrentRow(mainWindow->ui->pagesTabWidget->currentIndex()+1);

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
        case ctList:
        {
            ui->typeComboBox->setCurrentIndex(5);

            ui->listDefaultComboBox->setEditText(((ListColumn*)aColumn->column)->mDefaultValue);
        }
        break;
        case ctExtendedList:
        {
            ui->typeComboBox->setCurrentIndex(6);

            ui->extendedListComboBox->setEditText(((ExtendedListColumn*)aColumn->column)->mDefaultValue);
        }
        break;
        case ctExpression:
        {
            ui->typeComboBox->setCurrentIndex(7);

            ui->expressionEdit->setText(((ExpressionColumn*)aColumn->column)->mDefaultValue);
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
            case 5:
            {
                ListColumn *aTypeColumn=new ListColumn();

                aTypeColumn->mDefaultValue=ui->listDefaultComboBox->currentText();

                aColumn.column=aTypeColumn;
            }
            break;
            case 6:
            {
                ExtendedListColumn *aTypeColumn=new ExtendedListColumn();

                aTypeColumn->mDefaultValue=ui->extendedListComboBox->currentText();

                aColumn.column=aTypeColumn;
            }
            break;
            case 7:
            {
                ExpressionColumn *aTypeColumn=new ExpressionColumn();

                aTypeColumn->mDefaultValue=ui->expressionEdit->text();

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
        aColumn.fontString="";
        aColumn.alignment=Qt::AlignTop | Qt::AlignLeft;
        aColumn.backgroundColorR=255;
        aColumn.backgroundColorG=255;
        aColumn.backgroundColorB=255;
        aColumn.textColorR=0;
        aColumn.textColorG=0;
        aColumn.textColorB=0;

        mTable->typeColumns.insert(mColumnIndex, aColumn);

        mTableWidget->insertColumn(mColumnIndex);
        mTableWidget->setHorizontalHeaderItem(mColumnIndex, new QTableWidgetItem(aColumn.name));

        QTableWidgetItem *aItem;
        QBrush aBrush(QColor(255, 255, 255));

        aItem=new QTableWidgetItem(aColumn.column->typeDescription());
        aItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
        aItem->setBackground(aBrush);

        mTableWidget->setItem(0, mColumnIndex, aItem);

        aItem=new QTableWidgetItem("Промежуточная строка");
        aItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
        aItem->setBackground(aBrush);

        mTableWidget->setItem(1, mColumnIndex, aItem);

        if (mTableWidget->columnCount()>1)
        {
            int firstCell=0;

            if (mColumnIndex==0)
            {
                firstCell++;
            }

            mTableWidget->item(1, mColumnIndex)->setFont(mTableWidget->item(1, firstCell)->font());
            mTableWidget->item(1, mColumnIndex)->setBackground(mTableWidget->item(1, firstCell)->background());
            mTableWidget->item(1, mColumnIndex)->setTextColor(mTableWidget->item(1, firstCell)->textColor());
            mTableWidget->item(1, mColumnIndex)->setTextAlignment(mTableWidget->item(1, firstCell)->textAlignment());

            mTableWidget->separate(1, firstCell);

            mTableWidget->unite(0, 1, mTableWidget->columnCount()-1, 1);
        }

        if (mTable->ui->dataTableWidget->columnCount()>1)
        {
            for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
            {
                if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
                {
                    mTable->ui->dataTableWidget->setSpan(i, 0, 1, 1);
                }
            }
        }

        mTable->ui->dataTableWidget->insertColumn(mColumnIndex);
        mTable->ui->dataTableWidget->setHorizontalHeaderItem(mColumnIndex, new QTableWidgetItem(aColumn.name));

        for (int i=mTable->ui->dataTableWidget->columnCount()-1; i>mColumnIndex; i--)
        {
            mTable->ui->dataTableWidget->setItemDelegateForColumn(i, mTable->ui->dataTableWidget->itemDelegateForColumn(i-1));
        }

        switch (ui->typeComboBox->currentIndex())
        {
            case 0:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new DoubleDelegate(mTable));
            }
            break;
            case 3:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new DateDelegate(mTable));
            }
            break;
            case 4:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new TimeDelegate(mTable));
            }
            break;
            case 5:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new ListDelegate(mTable));
            }
            break;
            default:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new StringDelegate(mTable));
            }
            break;
        }

        for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
        {
            QTableWidgetItem *aItem=new QTableWidgetItem();

            aItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
            aItem->setBackground(QBrush(QColor(255, 255, 255)));

            if (mTable->ui->dataTableWidget->itemDelegateForRow(i)==0)
            {
                switch (aColumn.column->type())
                {
                    case ctInteger:
                    {
                        if (((IntegerColumn*)aColumn.column)->mIsAutoInc)
                        {
                            int id=1;

                            for (int j=0; j<i; j++)
                            {
                                if (mTable->ui->dataTableWidget->itemDelegateForRow(j)==0)
                                {
                                    id++;
                                }
                            }

                            aItem->setText(QString::number(id));
                        }
                        else
                        {
                            aItem->setText(QString::number((((IntegerColumn*)aColumn.column)->mDefaultValue)));
                        }
                    }
                    break;
                    case ctString:
                    {
                        aItem->setText((((StringColumn*)aColumn.column)->mDefaultValue));
                    }
                    break;
                    case ctBool:
                    {
                        if (((BoolColumn*)aColumn.column)->mDefaultValue)
                        {
                            aItem->setCheckState(Qt::Checked);
                        }
                        else
                        {
                            aItem->setCheckState(Qt::Unchecked);
                        }
                    }
                    break;
                    case ctDate:
                    {
                        aItem->setText(((DateColumn*)aColumn.column)->mDefaultValue.toString("dd.MM.yyyy"));
                    }
                    break;
                    case ctTime:
                    {
                        aItem->setText(((TimeColumn*)aColumn.column)->mDefaultValue.toString("hh:mm:ss"));
                    }
                    break;
                    case ctList:
                    {
                        aItem->setText((((ListColumn*)aColumn.column)->mDefaultValue));
                    }
                    break;
                    case ctExtendedList:
                    {
                        aItem->setText((((ExtendedListColumn*)aColumn.column)->mDefaultValue));
                    }
                    break;
                    case ctExpression:
                    {
                        aItem->setText((((ExpressionColumn*)aColumn.column)->mDefaultValue));
                    }
                    break;
                    default:
                    break;
                }
            }

            mTable->ui->dataTableWidget->setItem(i, mColumnIndex, aItem);
        }

        if (mTable->ui->dataTableWidget->columnCount()>1)
        {
            for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
            {
                if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
                {
                    mTable->ui->dataTableWidget->setSpan(i, 0, 1, mTable->ui->dataTableWidget->columnCount());
                }
            }
        }
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

void ColumnEditDialog::on_listLinkPagesListWidget_currentRowChanged(int currentRow)
{
    ui->listLinkVariablesListWidget->clear();

    if (currentRow<0)
    {
        return;
    }

    if (currentRow==0)
    {
        for (int i=0; i<globalDialog->variables.length(); i++)
        {
            if (globalDialog->variables.at(i)->inherits("VariableListFrame"))
            {
                ui->listLinkVariablesListWidget->addItem(globalDialog->variables.at(i)->variableName());
            }
        }
    }
    else
    {
        PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(currentRow-1);

        for (int i=0; i<aPage->variables.length(); i++)
        {
            if (aPage->variables.at(i)->inherits("VariableListFrame"))
            {
                ui->listLinkVariablesListWidget->addItem(aPage->variables.at(i)->variableName());
            }
        }
    }
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
