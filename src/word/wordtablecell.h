#ifndef WORDTABLECELL_H
#define WORDTABLECELL_H

#include "wordworkbase.h"
#include "wordrange.h"

class WordTableCell : public WordWorkBase
{
    Q_OBJECT
public:
    explicit WordTableCell(QAxObject *aBaseObject, QObject *parent = 0);

    WordRange* range();
};

#endif // WORDTABLECELL_H
