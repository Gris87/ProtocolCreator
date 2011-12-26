#ifndef TIMECOLUMN_H
#define TIMECOLUMN_H

#include <QDataStream>

#include "src/column_types/columntype.h"

#include <QTime>

class TimeColumn: public ColumnType
{
public:
    QTime mDefaultValue;

    explicit TimeColumn();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    QString typeDescription();
};

#endif // TIMECOLUMN_H
