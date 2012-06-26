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
    int curRow=currentRow();
    int curCol=currentColumn();

    if (
        itemDelegateForRow(curRow)
        ||
        (
         aTable->typeColumns.at(curCol).column->type()!=ctBool
         &&
         (
          aTable->typeColumns.at(curCol).column->type()!=ctExpression
          ||
          ((ExpressionColumn*)(aTable->typeColumns.at(curCol).column))->mAllowModify
         )
         &&
         (
          aTable->typeColumns.at(curCol).column->type()!=ctInteger
          ||
          !((IntegerColumn*)(aTable->typeColumns.at(curCol).column))->mIsAutoInc
         )
        )
       )
    {
        QTableWidget::commitData(editor);

        if (curRow<rowCount()-1)
        {
            lastRow=curRow;
            lastCol=curCol;
            QTimer::singleShot(0, this, SLOT(jumpToNext()));
        }
    }
}

void DataTable::jumpToNext()
{
    if (lastRow==currentRow() && lastCol==currentColumn())
    {
        setCurrentCell(lastRow+1, lastCol);
    }
}

void DataTable::keyPressEvent(QKeyEvent *event)
{
    if (event==QKeySequence::Copy)
    {
        QList<QTableWidgetItem *> aItems=selectedItems();

        if (aItems.length()>0)
        {
            int aLeft=aItems.at(0)->column();
            int aTop=aItems.at(0)->row();
            int aRight=aLeft;
            int aBottom=aTop;

            for (int i=1; i<aItems.length(); i++)
            {
                int aRow=aItems.at(i)->row();
                int aColumn=aItems.at(i)->column();

                if (aRow<aTop)
                {
                    aTop=aRow;
                }
                else
                if (aRow>aBottom)
                {
                    aBottom=aRow;
                }

                if (aColumn<aLeft)
                {
                    aLeft=aColumn;
                }
                else
                if (aColumn>aRight)
                {
                    aRight=aColumn;
                }
            }

            for (int i=aTop; i<=aBottom; i++)
            {
                for (int j=aLeft; j<=aRight; j++)
                {
                    if (!item(i, j)->isSelected())
                    {
                        return;
                    }
                }
            }

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
        if (editTriggers()==NoEditTriggers)
        {
            return;
        }

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
            VariableExtendedListFrame *aTable=(VariableExtendedListFrame*)mTable;

            aTable->ui->deleteRowButton->click();
        }
    }
    else
    {
        QTableWidget::keyPressEvent(event);
    }
}

void DataTable::pasteData()
{
    if (editTriggers()==NoEditTriggers)
    {
        return;
    }

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

    VariableExtendedListFrame *aTable=(VariableExtendedListFrame*)mTable;

    for (int i=0; i<cells.length(); i++)
    {
        if (cells.at(i).length()==1 && cells.at(i).at(0).startsWith("#### "))
        {
            QString aText=cells.at(i).at(0);
            aText.remove(0, 5);

            if (itemDelegateForRow(curRow+i)==0)
            {
                for (int j=0; j<totalCol; j++)
                {
                    delete item(curRow+i, j);
                }

                aTable->setItemsForMiddleRow(curRow+i);
            }

            item(curRow+i, 0)->setText(aText);
        }
        else
        {
            if (itemDelegateForRow(curRow+i))
            {
                curCol=0;

                delete itemDelegateForRow(curRow+i);

                for (int j=curRow+i; j<totalRow-1; j++)
                {
                    setItemDelegateForRow(j, itemDelegateForRow(j+1));
                }

                removeRow(curRow+i);
                aTable->insertRow(curRow+i);
            }

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
                                if (((IntegerColumn*)(aTable->typeColumns.at(curCol+j).column))->mSplitRows)
                                {
                                    aItem->setText(QString::number(aValue, 'f', ((IntegerColumn*)(aTable->typeColumns.at(curCol+j).column))->mDecimals));
                                }
                                else
                                {
                                    aItem->setText(
                                                   ((IntegerColumn*)(aTable->typeColumns.at(curCol+j).column))->mPrefix+
                                                   QString::number(aValue, 'f', ((IntegerColumn*)(aTable->typeColumns.at(curCol+j).column))->mDecimals)+
                                                   ((IntegerColumn*)(aTable->typeColumns.at(curCol+j).column))->mPostfix
                                                  );
                                }
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
                        QTime aTime=QTime::fromString(aText, "h:mm:ss");

                        if (aTime.isValid())
                        {
                            aItem->setText(aTime.toString("hh:mm:ss"));
                        }
                    }
                    break;
                    case ctExpression:
                    {
                        if (((ExpressionColumn*)(aTable->typeColumns.at(curCol+j).column))->mAllowModify)
                        {
                            aItem->setText(aText);
                        }
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
