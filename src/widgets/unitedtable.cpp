#include "unitedtable.h"

UnitedTable::UnitedTable(QWidget *parent) :
        CopyableTable(parent)
{
}

void UnitedTable::unite(int left, int top, int right, int bottom)
{
    QString aText="";

    for (int i=top; i<=bottom; i++)
    {
        for (int j=left; j<=right; j++)
        {
            QTableWidgetItem *aItem=item(i, j);

            if (aItem->text()!="")
            {
                if (aText=="")
                {
                    aText=aItem->text();
                }

                aItem->setText("");
            }
        }
    }

    item(top, left)->setText(aText);

    setSpan(top, left, bottom-top+1, right-left+1);
}

void UnitedTable::separate(int row, int column)
{
    setSpan(row, column, 1, 1);
}

void UnitedTable::uniteSelection()
{
    QList<QTableWidgetItem *> aItems=selectedItems();

    int startX=aItems.at(0)->column();
    int startY=aItems.at(0)->row();

    int leftLimit=startX;
    int topLimit=startY;
    int rightLimit=startX;
    int bottomLimit=startY;

    while (leftLimit>0 && item(startY, leftLimit-1)->isSelected())
    {
        leftLimit--;
    }

    while (topLimit>0 && item(topLimit-1, startX)->isSelected())
    {
        topLimit--;
    }

    while (rightLimit<columnCount()-1 && item(startY, rightLimit+1)->isSelected())
    {
        rightLimit++;
    }

    while (bottomLimit<rowCount()-1 && item(bottomLimit+1, startX)->isSelected())
    {
        bottomLimit++;
    }

    for (int i=topLimit; i<=bottomLimit; i++)
    {
        for (int j=leftLimit; j<=rightLimit; j++)
        {
            QTableWidgetItem *aItem=item(i, j);

            if (!aItem->isSelected())
            {
                QMessageBox::information(this, "Оьединение ячеек", "Выделение не образует прямоугольную зону");
                return;
            }

            aItems.removeOne(aItem);
        }
    }

    if (aItems.length()>0)
    {
        QMessageBox::information(this, "Оьединение ячеек", "Выделение не образует прямоугольную зону");
        return;
    }

    separateSelection();

    unite(leftLimit, topLimit, rightLimit, bottomLimit);
}

void UnitedTable::separateSelection()
{
    QList<QTableWidgetItem *> aItems=selectedItems();

    for (int i=0; i<aItems.length(); i++)
    {
        separate(aItems.at(i)->row(), aItems.at(i)->column());
    }
}
