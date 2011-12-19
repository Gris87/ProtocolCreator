#ifndef UNITEDTABLE_H
#define UNITEDTABLE_H

#include "src/widgets/copyabletable.h"

#include <qdebug.h>

class UnitedTable : public CopyableTable
{
    Q_OBJECT
public:
    QList<QRect> unions;

    explicit UnitedTable(QWidget *parent = 0);

    int indexOfUnion(int row, int column);

    void unite(int left, int top, int right, int bottom);
    void separate(int row, int column);

protected:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // UNITEDTABLE_H
