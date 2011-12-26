#ifndef BOOLCOLUMN_H
#define BOOLCOLUMN_H

#include <QDataStream>

#include "src/column_types/columntype.h"

class BoolColumn: public ColumnType
{
public:
    bool mDefaultValue;

    explicit BoolColumn();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);
};

#endif // BOOLCOLUMN_H
