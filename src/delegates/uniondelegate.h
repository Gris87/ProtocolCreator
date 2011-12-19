#ifndef UNIONDELEGATE_H
#define UNIONDELEGATE_H

#include <QStyledItemDelegate>

#include <QPainter>
#include <QPen>
#include "src/widgets/unitedtable.h"

class UnionDelegate : public QStyledItemDelegate
{
public:
    UnitedTable* mTable;
    UnionDelegate(UnitedTable* aTable);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
    QPen _gridPen;
};


#endif // UNIONDELEGATE_H
