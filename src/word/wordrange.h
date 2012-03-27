#ifndef WORDRANGE_H
#define WORDRANGE_H

#include "wordworkbase.h"

class WordTables;

class WordRange : public WordWorkBase
{
    Q_OBJECT
public:
    explicit WordRange(QAxObject *aBaseObject, QObject *parent = 0);

    WordTables* tables();

    void InsertAfter(QString aText);
};

#endif // WORDRANGE_H
