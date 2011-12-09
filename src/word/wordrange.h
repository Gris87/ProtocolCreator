#ifndef WORDRANGE_H
#define WORDRANGE_H

#include "wordworkbase.h"

class WordRange : public WordWorkBase
{
    Q_OBJECT
public:
    explicit WordRange(QAxObject *aBaseObject, QObject *parent = 0);

    void InsertAfter(QString aText);
};

#endif // WORDRANGE_H
