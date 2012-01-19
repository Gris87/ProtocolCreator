#ifndef LISTCOLUMN_H
#define LISTCOLUMN_H

#include <QDataStream>

#include "src/column_types/columntype.h"

class ListColumn: public ColumnType
{
public:
    QString mDefaultValue;

    explicit ListColumn();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    QString typeDescription();
    EColumnType type();
};

#endif // LISTCOLUMN_H
