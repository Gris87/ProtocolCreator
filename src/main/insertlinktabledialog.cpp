#include "src/other/global.h"

InsertLinkTableDialog::InsertLinkTableDialog(VariableExtendedListFrame *aSourceTable, VariableExtendedListFrame *aDestTable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertLinkTableDialog)
{
    ui->setupUi(this);

    mSourceTable=aSourceTable;
    mDestTable=aDestTable;
    mBeginPos=0;

    ui->sourceTableWidget->setRowCount(mSourceTable->ui->dataTableWidget->rowCount());
    ui->sourceTableWidget->setColumnCount(mSourceTable->ui->dataTableWidget->columnCount());

    for (int i=0; i<ui->sourceTableWidget->rowCount(); i++)
    {
        for (int j=0; j<ui->sourceTableWidget->columnCount(); j++)
        {
            ui->sourceTableWidget->setItem(i, j, mSourceTable->ui->dataTableWidget->item(i, j)->clone());

            if (ui->sourceTableWidget->item(i, j)->data(Qt::CheckStateRole).isValid())
            {
                ui->sourceTableWidget->item(i, j)->setFlags(ui->sourceTableWidget->item(i, j)->flags() & ~Qt::ItemIsUserCheckable);
            }
        }

        if (mSourceTable->ui->dataTableWidget->itemDelegateForRow(i))
        {
            ui->sourceTableWidget->setItemDelegateForRow(i, new StringDelegate(this));
            ui->sourceTableWidget->setSpan(i, 0, 1, ui->sourceTableWidget->columnCount());
        }
    }

    for (int i=0; i<ui->sourceTableWidget->columnCount(); i++)
    {
        ui->sourceTableWidget->setHorizontalHeaderItem(i, mSourceTable->ui->dataTableWidget->horizontalHeaderItem(i)->clone());
    }

    if (mDestTable->ui->dataTableWidget->currentRow()<0)
    {
        ui->beforeRadioButton->setEnabled(false);
        ui->afterRadioButton->setEnabled(false);
    }
}

InsertLinkTableDialog::~InsertLinkTableDialog()
{
    delete ui;
}

void InsertLinkTableDialog::on_closeButton_clicked()
{
    reject();
}

void InsertLinkTableDialog::on_addButton_clicked()
{
    QList<QTableWidgetItem *> aItems=ui->sourceTableWidget->selectedItems();
    QList<int> rows;

    for (int i=0; i<aItems.length(); i++)
    {
        if (!rows.contains(aItems.at(i)->row()))
        {
            rows.append(aItems.at(i)->row());
        }
    }

    for (int e=0; e<rows.length()-1; e++)
    {
        int min=rows.at(e);
        int minIndex=e;

        for (int i=e+1; i<rows.length(); i++)
        {
            if (rows.at(i)<min)
            {
                min=rows.at(i);
                minIndex=i;
            }
        }

        rows.swap(e, minIndex);
    }

    for (int i=0; i<rows.length(); i++)
    {
        int aSourceRow=rows.at(i);
        int aDestRow=-1;

        if (ui->beginRadioButton->isChecked())
        {
            aDestRow=mBeginPos;
            mBeginPos++;
        }
        else
        if (ui->beforeRadioButton->isChecked())
        {
            aDestRow=mDestTable->ui->dataTableWidget->currentRow();
            mBeginPos=0;
        }
        else
        if (ui->afterRadioButton->isChecked())
        {
            aDestRow=mDestTable->ui->dataTableWidget->currentRow()+1;
            mBeginPos=0;
        }
        else
        if (ui->endRadioButton->isChecked())
        {
            aDestRow=mDestTable->ui->dataTableWidget->rowCount();
            mBeginPos=0;
        }

        if (ui->sourceTableWidget->itemDelegateForRow(aSourceRow))
        {
            mDestTable->insertMiddleRow(aDestRow);

            mDestTable->ui->dataTableWidget->item(aDestRow, 0)->setText(ui->sourceTableWidget->item(aSourceRow, 0)->text());
        }
        else
        {
            mDestTable->insertRow(aDestRow);

            for (int j=0; j<ui->sourceTableWidget->columnCount() && j<mDestTable->ui->dataTableWidget->columnCount(); j++)
            {
                EColumnType aDestColumnType=mDestTable->typeColumns.at(j).column->type();
                EColumnType aSourceColumnType=mSourceTable->typeColumns.at(j).column->type();

                if (aDestColumnType==ctInteger)
                {
                    if (!((IntegerColumn*)mDestTable->typeColumns.at(j).column)->mIsAutoInc)
                    {
                        if (aSourceColumnType==ctInteger)
                        {
                            QString aText=ui->sourceTableWidget->item(aSourceRow, j)->text();

                            int removeBefore=((IntegerColumn*)mSourceTable->typeColumns.at(j).column)->mPrefix.length();
                            int removeAfter=((IntegerColumn*)mSourceTable->typeColumns.at(j).column)->mPostfix.length();

                            aText.remove(aText.length()-removeAfter, removeAfter);
                            aText.remove(0, removeBefore);

                            double aValue=aText.toDouble();

                            mDestTable->ui->dataTableWidget->item(aDestRow, j)->setText(
                                                                                        ((IntegerColumn*)mDestTable->typeColumns.at(j).column)->mPrefix+
                                                                                        QString::number(aValue, 'f', ((IntegerColumn*)mDestTable->typeColumns.at(j).column)->mDecimals)+
                                                                                        ((IntegerColumn*)mDestTable->typeColumns.at(j).column)->mPostfix);
                        }
                        else
                        {
                            bool ok=true;
                            double aValue;

                            if (aSourceColumnType==ctBool)
                            {
                                if (ui->sourceTableWidget->item(aSourceRow, j)->checkState()==Qt::Checked)
                                {
                                    aValue=1;
                                }
                                else
                                {
                                    aValue=0;
                                }
                            }
                            else
                            {
                                aValue=ui->sourceTableWidget->item(aSourceRow, j)->text().toDouble(&ok);
                            }

                            if (ok)
                            {
                                mDestTable->ui->dataTableWidget->item(aDestRow, j)->setText(
                                                                                            ((IntegerColumn*)mDestTable->typeColumns.at(j).column)->mPrefix+
                                                                                            QString::number(aValue, 'f', ((IntegerColumn*)mDestTable->typeColumns.at(j).column)->mDecimals)+
                                                                                            ((IntegerColumn*)mDestTable->typeColumns.at(j).column)->mPostfix);
                            }
                        }
                    }
                }
                else
                if (aDestColumnType==ctString || aDestColumnType==ctList || aDestColumnType==ctExtendedList)
                {
                    if (aSourceColumnType==ctBool)
                    {
                        if (ui->sourceTableWidget->item(aSourceRow, j)->checkState()==Qt::Checked)
                        {
                            mDestTable->ui->dataTableWidget->item(aDestRow, j)->setText("1");
                        }
                        else
                        {
                            mDestTable->ui->dataTableWidget->item(aDestRow, j)->setText("0");
                        }
                    }
                    else
                    {
                        mDestTable->ui->dataTableWidget->item(aDestRow, j)->setText(ui->sourceTableWidget->item(aSourceRow, j)->text());
                    }
                }
                else
                if (aDestColumnType==ctBool)
                {
                    if (aSourceColumnType==ctBool)
                    {
                        mDestTable->ui->dataTableWidget->item(aDestRow, j)->setCheckState(ui->sourceTableWidget->item(aSourceRow, j)->checkState());
                    }
                    else
                    {
                        if (ui->sourceTableWidget->item(aSourceRow, j)->text()=="1")
                        {
                            mDestTable->ui->dataTableWidget->item(aDestRow, j)->setCheckState(Qt::Checked);
                        }
                        else
                        if (ui->sourceTableWidget->item(aSourceRow, j)->text()=="0")
                        {
                            mDestTable->ui->dataTableWidget->item(aDestRow, j)->setCheckState(Qt::Unchecked);
                        }
                    }
                }
                else
                if (aDestColumnType==ctDate)
                {
                    QDate aDate=QDate::fromString(ui->sourceTableWidget->item(aSourceRow, j)->text(), "dd.MM.yyyy");

                    if (aDate.isValid())
                    {
                        mDestTable->ui->dataTableWidget->item(aDestRow, j)->setText(ui->sourceTableWidget->item(aSourceRow, j)->text());
                    }
                }
                else
                if (aDestColumnType==ctTime)
                {
                    QTime aTime=QTime::fromString(ui->sourceTableWidget->item(aSourceRow, j)->text(), "hh:mm:ss");

                    if (aTime.isValid())
                    {
                        mDestTable->ui->dataTableWidget->item(aDestRow, j)->setText(ui->sourceTableWidget->item(aSourceRow, j)->text());
                    }
                }
            }
        }
    }
}

void InsertLinkTableDialog::on_sourceTableWidget_doubleClicked(const QModelIndex &index)
{
    on_addButton_clicked();
}
