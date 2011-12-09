#ifndef WORDOPTIONS_H
#define WORDOPTIONS_H

#include "wordworkbase.h"

class WordOptions : public WordWorkBase
{
    Q_OBJECT
public:
    explicit WordOptions(QAxObject *aBaseObject, QObject *parent = 0);

    void SetCheckSpellingAsYouType(bool CheckSpellingAsYouType);

    bool CheckSpellingAsYouType();
};

#endif // WORDOPTIONS_H
