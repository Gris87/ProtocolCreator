#ifndef UNITEDTABLE_H
#define UNITEDTABLE_H

#include "src/widgets/copyabletable.h"

#include <QMessageBox>

class UnitedTable : public CopyableTable
{
    Q_OBJECT
public:
    explicit UnitedTable(QWidget *parent = 0);

    void unite(int left, int top, int right, int bottom);
    void separate(int row, int column);

public slots:
    void uniteSelection();
    void separateSelection();
};

#endif // UNITEDTABLE_H
