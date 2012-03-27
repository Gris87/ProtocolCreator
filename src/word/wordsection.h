#ifndef WORDSECTION_H
#define WORDSECTION_H

#include "wordworkbase.h"
#include "wordrange.h"

class WordSection : public WordWorkBase
{
    Q_OBJECT
public:
    explicit WordSection(QAxObject *aBaseObject, QObject *parent = 0);

    WordRange* range();
};

#endif // WORDSECTION_H
