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

    qDebug()<<rows;

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

            mDestTable->ui->dataTableWidget->item(aDestRow, 0)->setText(ui->sourceTableWidget->item(aSourceRow,0)->text());
        }
        else
        {
            mDestTable->insertRow(aDestRow);

            for (int j=0; j<ui->sourceTableWidget->columnCount() && j<mDestTable->ui->dataTableWidget->columnCount(); j++)
            {

            }
        }
    }
}

void InsertLinkTableDialog::on_sourceTableWidget_doubleClicked(const QModelIndex &index)
{
    on_addButton_clicked();
}
