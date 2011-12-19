#ifndef UNITEDTABLE_H
#define UNITEDTABLE_H

#include "src/widgets/copyabletable.h"

class UnitedTable : public CopyableTable
{
    Q_OBJECT
public:
    QList<QRect> unions;

    explicit UnitedTable(QWidget *parent = 0);

    void unite(int left, int top, int right, int bottom);
    void separate(int row, int column);

    void paintEvent(QPaintEvent *e);
};

#endif // UNITEDTABLE_H
