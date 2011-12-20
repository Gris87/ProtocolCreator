#include "uniondelegate.h"

UnionDelegate::UnionDelegate(UnitedTable* aTable)
{
    mTable=aTable;

    QColor gridColor(120, 120, 120);
    _gridPen = QPen(gridColor, 0, mTable->gridStyle());

    mTable->setShowGrid(false);
}

void UnionDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    bool drawLeft=true;
    bool drawTop=true;
    bool drawRight=true;
    bool drawBottom=true;

    int unionIndex=mTable->indexOfUnion(index.row(), index.column());

    if (unionIndex>=0)
    {
        drawLeft   = index.column() == mTable->unions.at(unionIndex).left();
        drawTop    = index.row()    == mTable->unions.at(unionIndex).top();
        drawRight  = index.column() == mTable->unions.at(unionIndex).right();
        drawBottom = index.row()    == mTable->unions.at(unionIndex).bottom();
    }

    if (drawLeft || drawTop || drawRight || drawBottom)
    {
        QPen oldPen = painter->pen();
        painter->setPen(_gridPen);

        if (drawLeft)
        {
            painter->drawLine(option.rect.left()-1, option.rect.bottom(), option.rect.left()-1, option.rect.top());
        }

        if (drawTop)
        {
            painter->drawLine(option.rect.left(), option.rect.top()-1, option.rect.right(), option.rect.top()-1);
        }

        if (drawRight)
        {
            painter->drawLine(option.rect.bottomRight(), option.rect.topRight());
        }

        if (drawBottom)
        {
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }

        painter->setPen(oldPen);
    }
}
