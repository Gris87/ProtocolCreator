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
                QString aRow="";

                if (itemDelegateForRow(i))
                {
                    if (columnCount()>0)
                    {
                        aRow="#### "+item(i, 0)->text();
                    }
                }
                else
                {
                    for (int j=aLeft; j<=aRight; j++)
                    {
                        if (aRow!="")
                        {
                            aRow.append("\t");
                        }

                        if (item(i, j)->data(Qt::CheckStateRole).isValid())
                        {
                            if (item(i, j)->checkState()==Qt::Checked)
                            {
                                aRow.append("1");
                            }
                            else
                            {
                                aRow.append("0");
                            }
                        }
                        else
                        {
                            aRow.append(item(i, j)->text());
                        }
                    }
                }

                if (aRow!="" && toClipBoard!="")
                {
                    toClipBoard.append("\n");
                }

                toClipBoard.append(aRow);
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
    int curRow=currentRow();

    if (curRow<0)
    {
        return;
    }

    int curCol=currentColumn();
    int totalRow=rowCount();
    int totalCol=columnCount();

    QString aClipboard=QApplication::clipboard()->text().replace("\r","");

    if (aClipboard.endsWith("\n"))
    {
        aClipboard.remove(aClipboard.length()-1, 1);
    }

    if (aClipboard=="")
    {
        return;
    }

    QStringList rows=aClipboard.split("\n");

    QList<QStringList> cells;

    for (int i=0; i<rows.length(); i++)
    {
        if (curRow+i<totalRow)
        {
            cells.append(rows.at(i).split("\t"));
        }
    }

    VariableExtendedListFrame* aTable=(VariableExtendedListFrame*)parent()->parent()->parent();

    for (int i=0; i<cells.length(); i++)
    {
        if (cells.at(i).length()==1 && cells.at(i).at(0).startsWith("#### "))
        {
            QString aText=cells.at(i).at(0);
            aText.remove(0, 5);

            QTableWidgetItem *aItem=item(curRow+i, 0);

            for (int j=1; j<totalCol; j++)
            {
                item(curRow+i, j)->setFont(aItem->font());
                item(curRow+i, j)->setBackground(aItem->background());
                item(curRow+i, j)->setTextColor(aItem->textColor());
                item(curRow+i, j)->setTextAlignment(aItem->textAlignment());
                item(curRow+i, j)->setText("");
            }

            setSpan(curRow+i, 0, 1, totalCol);
            aItem->setText(aText);

            if (aTable->mLinkForMiddleRow=="")
            {
                setItemDelegateForRow(curRow+i, new StringDelegate(aTable));
            }
            else
            {
                setItemDelegateForRow(curRow+i, new ListDelegate(aTable->mLinkForMiddleRow, aTable));
            }
        }
        else
        {
            for (int j=0; j<cells.at(i).length() && curCol+j<totalCol; j++)
            {
                QString aText=cells.at(i).at(j);
                QTableWidgetItem *aItem=item(curRow+i, curCol+j);

                switch (aTable->typeColumns.at(curCol+j).column->type())
                {
                    case ctInteger:
                    {
                        if (!((IntegerColumn*)(aTable->typeColumns.at(curCol+j).column))->mIsAutoInc)
                        {
                            while (aText.length()>0 && !aText.at(aText.length()-1).isNumber())
                            {
                                aText.remove(aText.length()-1, 1);
                            }

                            while (aText.length()>0 && !aText.at(0).isNumber())
                            {
                                aText.remove(0, 1);
                            }

                            bool ok;
                            double aValue=aText.toDouble(&ok);

                            if (ok)
                            {
                                aItem->setText(
                                               ((IntegerColumn*)(aTable->typeColumns.at(curCol+j).column))->mPrefix+
                                               QString::number(aValue, 'f', ((IntegerColumn*)(aTable->typeColumns.at(curCol+j).column))->mDecimals)+
                                               ((IntegerColumn*)(aTable->typeColumns.at(curCol+j).column))->mPostfix
                                              );
                            }
                        }
                    }
                    break;
                    case ctString:
                    case ctList:
                    case ctExtendedList:
                    {
                        aItem->setText(aText);
                    }
                    break;
                    case ctBool:
                    {
                        if (aText=="1")
                        {
                            aItem->setCheckState(Qt::Checked);
                        }
                        else
                        if (aText=="0")
                        {
                            aItem->setCheckState(Qt::Unchecked);
                        }
                    }
                    break;
                    case ctDate:
                    {
                        QDate aDate=QDate::fromString(aText, "dd.MM.yyyy");

                        if (aDate.isValid())
                        {
                            aItem->setText(aText);
                        }
                    }
                    break;
                    case ctTime:
                    {
                        QTime aTime=QTime::fromString(aText, "hh:mm:ss");

                        if (aTime.isValid())
                        {
                            aItem->setText(aText);
                        }
                    }
                    break;
                    case ctExpression:
                    {
                        // Nothing
                    }
                    break;
                    default:
                    {
                        throw "Unknown column type";
                    }
                }
            }
        }
    }

    for (int i=0; i<columnCount(); i++)
    {
        if (
            aTable->typeColumns.at(i).column->type()==ctInteger
            &&
            ((IntegerColumn*)aTable->typeColumns.at(i).column)->mIsAutoInc
           )
        {
            int id=1;

            for (int j=0; j<rowCount(); j++)
            {
                if (itemDelegateForRow(j)==0)
                {
                    item(j,i)->setText(QString::number(id));
                    id++;
                }
            }
        }
    }
}
