#ifndef COLUMNTYPE_H
#define COLUMNTYPE_H

#include <QDataStream>

class ColumnType
{
public:
    explicit ColumnType();

    virtual void saveToStream(QDataStream &aStream);
    virtual void loadFromStream(QDataStream &aStream);

    virtual QString typeDescription();
};

#endif // COLUMNTYPE_H
