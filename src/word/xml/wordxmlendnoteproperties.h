#ifndef WORDXMLENDNOTEPROPERTIES_H
#define WORDXMLENDNOTEPROPERTIES_H

#include <QTextStream>

#include "wordxmlendnote.h"

class WordXMLEndnoteProperties
{
public:
    explicit WordXMLEndnoteProperties();
    ~WordXMLEndnoteProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    WordXMLEndnote* add(QString aType);

    WordXMLEndnote* copy(QString aType, QString aSecondType);
    WordXMLEndnote* copy(int index, QString aSecondType);
    WordXMLEndnote* copy(WordXMLEndnote* aEndnote, QString aSecondType);

    WordXMLEndnote* get(QString aType);
    WordXMLEndnote* get(int index);

    void remove(QString aType);
    void remove(int index);
    void remove(WordXMLEndnote* aEndnote);

    int count();

private:
    QList<WordXMLEndnote*> mList;

    void clear();
};

#endif // WORDXMLENDNOTEPROPERTIES_H
