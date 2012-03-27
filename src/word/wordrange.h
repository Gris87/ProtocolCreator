#ifndef WORDRANGE_H
#define WORDRANGE_H

#include "wordworkbase.h"

class WordTables;

enum WdStatistic
{
    wdStatisticWords,
    wdStatisticLines,
    wdStatisticPages,
    wdStatisticCharacters,
    wdStatisticParagraphs,
    wdStatisticCharactersWithSpaces,
    wdStatisticFarEastCharacters
};

class WordRange : public WordWorkBase
{
    Q_OBJECT
public:
    explicit WordRange(QAxObject *aBaseObject, QObject *parent = 0);

    WordTables* tables();

    int computeStatistics(WdStatistic Statistic);

    void InsertAfter(QString aText);
};

#endif // WORDRANGE_H
