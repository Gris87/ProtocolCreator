#ifndef WORDWORKBASE_H
#define WORDWORKBASE_H

#include "wordbase.h"

class WordWorkBase : public WordBase
{
    Q_OBJECT
public:
    explicit WordWorkBase(QAxObject *aBaseObject, QObject *parent = 0);
};

#endif // WORDWORKBASE_H
