#ifndef WORDSECTIONS_H
#define WORDSECTIONS_H

#include "wordworkbase.h"
#include "wordsection.h"

#include <QDir>

class WordSections : public WordWorkBase
{
    Q_OBJECT
public:
    explicit WordSections(QAxObject *aBaseObject, QObject *parent = 0);

    WordSection* add();
    WordSection* item(int index);
    int count();
};

#endif // WORDSECTIONS_H
