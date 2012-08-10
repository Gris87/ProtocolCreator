#ifndef WORDTABLE_H
#define WORDTABLE_H

#include "wordworkbase.h"
#include "wordtablerows.h"
#include "wordtablecolumns.h"
#include "wordtablecell.h"

class WordTable : public WordWorkBase
{
    Q_OBJECT
public:
    explicit WordTable(QAxObject *aBaseObject, QObject *parent = 0);

    WordTableRows* rows();
    WordTableColumns* columns();
    WordTableCell* cell(int row, int column);
};

#endif // WORDTABLE_H
