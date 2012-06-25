#ifndef EXTENDEDLISTCOLUMN_H
#define EXTENDEDLISTCOLUMN_H

#include <QDataStream>

#include "src/column_types/columntype.h"

class ExtendedListColumn: public ColumnType
{
public:
    QString mDefaultValue;
    QString mLinkComponent;

    explicit ExtendedListColumn();

    void init();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    QString typeDescription();
    EColumnType type();
};

#endif // EXTENDEDLISTCOLUMN_H
