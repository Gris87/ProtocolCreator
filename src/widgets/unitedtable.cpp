#include "unitedtable.h"

UnitedTable::UnitedTable(QWidget *parent) :
        CopyableTable(parent)
{
}

void UnitedTable::unite(int left, int top, int right, int bottom)
{
    QRect aRect;

    aRect.setCoords(left, top, right, bottom);

    unions.append(aRect);
}

void UnitedTable::separate(int row, int column)
{
    for (int i=0; i<unions.length(); i++)
    {
        if (unions.at(i).contains(column, row))
        {
            unions.removeAt(i);
            break;
        }
    }
}

void UnitedTable::paintEvent(QPaintEvent *e)
{
    QTableWidget::paintEvent(e);
}
