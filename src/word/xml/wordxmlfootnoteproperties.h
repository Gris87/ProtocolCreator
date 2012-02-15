#ifndef WORDXMLFOOTNOTEPROPERTIES_H
#define WORDXMLFOOTNOTEPROPERTIES_H

#include <QTextStream>

#include "wordxmlfootnote.h"

class WordXMLFootnoteProperties
{
public:
    explicit WordXMLFootnoteProperties();
    ~WordXMLFootnoteProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    WordXMLFootnote* add(QString aType);

    WordXMLFootnote* copy(QString aType, QString aSecondType);
    WordXMLFootnote* copy(int index, QString aSecondType);
    WordXMLFootnote* copy(WordXMLFootnote* aFootnote, QString aSecondType);

    WordXMLFootnote* get(QString aType);
    WordXMLFootnote* get(int index);

    void remove(QString aType);
    void remove(int index);
    void remove(WordXMLFootnote* aFootnote);

    int count();

private:
    QList<WordXMLFootnote*> mList;

    void clear();
};

#endif // WORDXMLFOOTNOTEPROPERTIES_H
