#ifndef WORDTABLES_H
#define WORDTABLES_H

#include "wordworkbase.h"
#include "wordrange.h"
#include "wordtable.h"

class WordTables : public WordWorkBase
{
    Q_OBJECT    
public:
    explicit WordTables(QAxObject *aBaseObject, QObject *parent = 0);

    WordTable* add(QVariant aRange, int NumRows, int NumColumns, bool DefaultTableBehavior = false, bool AutoFitBehavior = false);
    WordTable* addOld(QVariant aRange, int NumRows, int NumColumns);
    WordTable* item(int index);
    int count();
};

#endif // WORDTABLES_H
