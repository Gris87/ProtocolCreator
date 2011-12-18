#ifndef UNITEDTABLE_H
#define UNITEDTABLE_H

#include "src/widgets/copyabletable.h"

class UnitedTable : public CopyableTable
{
    Q_OBJECT
public:
    explicit UnitedTable(QWidget *parent = 0);
};

#endif // UNITEDTABLE_H
