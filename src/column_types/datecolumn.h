#ifndef DATECOLUMN_H
#define DATECOLUMN_H

#include <QDataStream>

#include "src/column_types/columntype.h"

#include <QDate>

class DateColumn: public ColumnType
{
public:
    QDate mDefaultValue;

    explicit DateColumn();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    QString typeDescription();
    EColumnType type();
};

#endif // DATECOLUMN_H
