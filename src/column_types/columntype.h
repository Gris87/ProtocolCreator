#ifndef COLUMNTYPE_H
#define COLUMNTYPE_H

#include <QDataStream>

class ColumnType
{
public:
    QString mName;

    explicit ColumnType();

    virtual void saveToStream(QDataStream &aStream);
    virtual void loadFromStream(QDataStream &aStream);
};

#endif // COLUMNTYPE_H
