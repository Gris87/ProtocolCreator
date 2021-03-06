#ifndef BOOLCOLUMN_H
#define BOOLCOLUMN_H

#include <QDataStream>

#include "src/column_types/columntype.h"

class BoolColumn: public ColumnType
{
public:
    bool mDefaultValue;

    explicit BoolColumn();

    void init();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    QString typeDescription();
    EColumnType type();
};

#endif // BOOLCOLUMN_H
