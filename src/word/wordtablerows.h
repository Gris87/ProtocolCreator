#ifndef WORDTABLEROWS_H
#define WORDTABLEROWS_H

#include "wordworkbase.h"

class WordTableRows : public WordWorkBase
{
    Q_OBJECT
public:
    explicit WordTableRows(QAxObject *aBaseObject, QObject *parent = 0);

    int count();
};

#endif // WORDTABLEROWS_H
