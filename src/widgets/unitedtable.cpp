#include "unitedtable.h"
#include "src/delegates/uniondelegate.h"

UnitedTable::UnitedTable(QWidget *parent) :
        CopyableTable(parent)
{
    setItemDelegate(new UnionDelegate(this));
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

    for (int i=0; i<unions.length(); i++)
    {
        if (aRect.contains(unions.at(i)))
        {
            unions.removeAt(i);
            i--;
        }
    }

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

void UnitedTable::insertRow(int row)
{
    for (int i=0; i<unions.length(); i++)
    {
        if (unions.at(i).top()>=row)
        {
            unions[i].setTop(unions.at(i).top()+1);
        }

        if (unions.at(i).bottom()>=row)
        {
            unions[i].setBottom(unions.at(i).bottom()+1);
        }
    }

    CopyableTable::insertRow(row);
}

void UnitedTable::insertColumn(int column)
{
    for (int i=0; i<unions.length(); i++)
    {
        if (unions.at(i).left()>=column)
        {
            unions[i].setLeft(unions.at(i).left()+1);
        }

        if (unions.at(i).right()>=column)
        {
            unions[i].setRight(unions.at(i).right()+1);
        }
    }

    CopyableTable::insertColumn(column);
}

void UnitedTable::removeRow(int row)
{
    for (int i=0; i<unions.length(); i++)
    {
        if (unions.at(i).top()>row)
        {
            unions[i].setTop(unions.at(i).top()-1);
        }

        if (unions.at(i).bottom()>=row)
        {
            unions[i].setBottom(unions.at(i).bottom()-1);
        }

        if (
            unions.at(i).top()>unions.at(i).bottom()
            ||
            (
             unions.at(i).left()==unions.at(i).right()
             &&
             unions.at(i).top()==unions.at(i).bottom()
            )
           )
        {
            unions.removeAt(i);
            i--;
        }
    }

    CopyableTable::removeRow(row);
}

void UnitedTable::removeColumn(int column)
{
    for (int i=0; i<unions.length(); i++)
    {
        if (unions.at(i).left()>column)
        {
            unions[i].setLeft(unions.at(i).left()-1);
        }

        if (unions.at(i).right()>=column)
        {
            unions[i].setRight(unions.at(i).right()-1);
        }

        if (
            unions.at(i).left()>unions.at(i).right()
            ||
            (
             unions.at(i).left()==unions.at(i).right()
             &&
             unions.at(i).top()==unions.at(i).bottom()
            )
           )
        {
            unions.removeAt(i);
            i--;
        }
    }

    CopyableTable::removeColumn(column);
}
