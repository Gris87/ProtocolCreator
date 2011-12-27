#ifndef COLUMNTYPE_H
#define COLUMNTYPE_H

#include <QDataStream>

enum EColumnType {ctNone, ctInteger, ctString, ctBool, ctDate, ctTime, ctList, ctExtendedList, ctExpression};

class ColumnType
{
public:
    explicit ColumnType();

    virtual void saveToStream(QDataStream &aStream);
    virtual void loadFromStream(QDataStream &aStream);

    virtual QString typeDescription();
    virtual EColumnType type();
};

#endif // COLUMNTYPE_H
