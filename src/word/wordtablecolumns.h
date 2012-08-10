#ifndef WORDTABLECOLUMNS_H
#define WORDTABLECOLUMNS_H

#include "wordworkbase.h"

class WordTableColumns : public WordWorkBase
{
    Q_OBJECT
public:
    explicit WordTableColumns(QAxObject *aBaseObject, QObject *parent = 0);

    int count();
};

#endif // WORDTABLECOLUMNS_H
