#include "src/other/global.h"

DataTable::DataTable(QWidget *parent) :
    QTableWidget(parent)
{
}

QModelIndex DataTable::indexFromItem(QTableWidgetItem *item) const
{
    return QTableWidget::indexFromItem(item);
}

void DataTable::commitData(QWidget *editor)
{
    VariableExtendedListFrame *aTable=(VariableExtendedListFrame*)mTable;

    if (
        itemDelegateForRow(currentRow())
        ||
        (
         aTable->typeColumns.at(currentColumn()).column->type()!=ctBool
         &&
         aTable->typeColumns.at(currentColumn()).column->type()!=ctExpression
         &&
         (
          aTable->typeColumns.at(currentColumn()).column->type()!=ctInteger
          ||
          !((IntegerColumn*)(aTable->typeColumns.at(currentColumn()).column))->mIsAutoInc
         )
        )
       )
    {
        QTableWidget::commitData(editor);
    }
}

void DataTable::keyPressEvent(QKeyEvent *event)
{
    if (event==QKeySequence::Copy)
    {
        QList<QTableWidgetSelectionRange> aRanges=selectedRanges();

        if (aRanges.length()>0)
        {
            int aLeft=aRanges.at(0).leftColumn();
            int aTop=aRanges.at(0).topRow();
            int aRight=aRanges.at(0).rightColumn();
            int aBottom=aRanges.at(0).bottomRow();

            QString toClipBoard="";

            for (int i=aTop; i<=aBottom; i++)
            {
                for (int j=aLeft; j<=aRight; j++)
                {
                    if (!isColumnHidden(j))
                    {
                        toClipBoard.append(item(i,j)->text());

                        toClipBoard.append("\t");
                    }
                }

                if (toClipBoard.endsWith("\t"))
                {
                    toClipBoard.remove(toClipBoard.length()-1, 1);
                }

                if (i<aBottom)
                {
                    toClipBoard.append("\n");
                }
            }

            while (toClipBoard.endsWith("\n"))
            {
                toClipBoard.remove(toClipBoard.length()-1, 1);
            }

            QApplication::clipboard()->setText(toClipBoard);
        }
    }
    else
    if (event==QKeySequence::Paste)
    {
        pasteData();
    }
    else
    if (event==QKeySequence::Delete)
    {
        QList<QTableWidgetSelectionRange> aRanges=selectedRanges();

        QList<int> aRows;

        for (int i=0; i<aRanges.length(); i++)
        {
            for (int j=aRanges.at(i).bottomRow(); j>=aRanges.at(i).topRow(); j--)
            {
                if (!aRows.contains(j))
                {
                    aRows.append(j);
                }
            }
        }

        if (
            (
             aRows.length()==1
             &&
             QMessageBox::question(this, protocolCreatorVersion, "Вы хотите удалить строку?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::Yes
            )
            ||
            (
             aRows.length()>=2
             &&
             aRows.length()<=4
             &&
             QMessageBox::question(this, protocolCreatorVersion, "Вы хотите удалить "+QString::number(aRows.length())+" строки?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::Yes
            )
            ||
            (
             aRows.length()>=5
             &&
             QMessageBox::question(this, protocolCreatorVersion, "Вы хотите удалить "+QString::number(aRows.length())+" строк?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::Yes
            )
           )
        {
            ((VariableExtendedListFrame*)parent()->parent()->parent())->ui->deleteRowButton->click();
        }
    }
    else
    {
        QTableWidget::keyPressEvent(event);
    }
}

void DataTable::pasteData()
{
    //VariableExtendedListFrame* aTable=(VariableExtendedListFrame*)parent()->parent()->parent();
}
