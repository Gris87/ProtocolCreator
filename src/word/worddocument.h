#ifndef WORDDOCUMENT_H
#define WORDDOCUMENT_H

#include "wordworkbase.h"
#include "wordsections.h"
#include "wordtables.h"
#include "wordrange.h"

class WordDocument : public WordWorkBase
{
    Q_OBJECT
public:
    explicit WordDocument(QAxObject *aBaseObject, QObject *parent = 0);

    WordSections* sections();

    WordTables* tables();

    WordRange* range();

    int fileFormat();
    void repaginate();
    void save();
    void saveAs(QString aFileName, int aFileFormat);
    void saveAs(QString aFileName);
    void saveAs2003(QString aFileName, int aDefaultFileFormat);
    void close();
};

#endif // WORDDOCUMENT_H
