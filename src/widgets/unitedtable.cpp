#include "unitedtable.h"

UnitedTable::UnitedTable(QWidget *parent) :
        CopyableTable(parent)
{
}

int UnitedTable::indexOfUnion(int row, int column)
{
    for (int i=0; i<unions.length(); i++)
    {
        if (unions.at(i).contains(column, row))
        {
            return i;
        }
    }

    return -1;
}

void UnitedTable::unite(int left, int top, int right, int bottom)
{
    QRect aRect;

    aRect.setCoords(left, top, right, bottom);

    unions.append(aRect);

    repaint();
}

void UnitedTable::separate(int row, int column)
{
    int index=indexOfUnion(row, column);

    if (index>=0)
    {
        unions.removeAt(index);

        repaint();
    }
}

void UnitedTable::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QItemSelection aNewSelected=selected;
    QItemSelection aNewDeselected=deselected;

    QModelIndexList aIndexes;

    aIndexes=deselected.indexes();

    for (int i=0; i<aIndexes.length(); i++)
    {
        int index=indexOfUnion(aIndexes.at(i).row(), aIndexes.at(i).column());

        if (index>=0)
        {
            /*
            QModelIndex topLeftIndex=indexFromItem(item(unions.at(index).top(), unions.at(index).left()));
            QModelIndex bottomRightIndex=indexFromItem(item(unions.at(index).bottom(), unions.at(index).right()));

            aNewDeselected.select(topLeftIndex, bottomRightIndex);

            QItemSelection aTempSelection(topLeftIndex, bottomRightIndex);

            aNewDeselected.merge(aTempSelection, QItemSelectionModel::Select);
            */

           // setSelection(unions.at(index), QItemSelectionModel::Deselect);
        }
    }

    aIndexes=selected.indexes();

    for (int i=0; i<aIndexes.length(); i++)
    {
        int index=indexOfUnion(aIndexes.at(i).row(), aIndexes.at(i).column());

        if (index>=0)
        {
    //        QModelIndex topLeftIndex=indexFromItem(item(unions.at(index).top(), unions.at(index).left()));
      //      QModelIndex bottomRightIndex=indexFromItem(item(unions.at(index).bottom(), unions.at(index).right()));

            //QItemSelection aTempSelection(topLeftIndex, bottomRightIndex);
            setSelection(unions.at(index), QItemSelectionModel::Select);

          //  aNewSelected.merge(aTempSelection, QItemSelectionModel::Select);

//            qDebug()<<"DSF"<<topLeftIndex.row()<<topLeftIndex.column()<<bottomRightIndex.row()<<bottomRightIndex.column()<<aNewSelected.indexes().length();
        }
    }

    /*
    aNewSelected.merge(aNewDeselected, QItemSelectionModel::Deselect);

    aIndexes=aNewSelected.indexes();

    for (int i=0; i<aIndexes.length(); i++)
    {
        item(aIndexes.at(i).row(), aIndexes.at(i).column())->setSelected(true);
    }

    aIndexes=aNewDeselected.indexes();

    for (int i=0; i<aIndexes.length(); i++)
    {
        item(aIndexes.at(i).row(), aIndexes.at(i).column())->setSelected(false);
    }
    */

    //CopyableTable::selectionChanged(aNewSelected, aNewDeselected);
    CopyableTable::selectionChanged(selected, deselected);
}
