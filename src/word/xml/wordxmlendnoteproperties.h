#ifndef WORDXMLENDNOTEPROPERTIES_H
#define WORDXMLENDNOTEPROPERTIES_H

#include "wordxmlbase.h"

#include "wordxmlendnote.h"

class WordXMLEndnoteProperties : public WordXMLBase
{
public:
    explicit WordXMLEndnoteProperties(WordXMLBase* aParent=0);
    ~WordXMLEndnoteProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

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

    void clear();

    WordXMLEndnoteProperties& operator=(const WordXMLEndnoteProperties &another);

private:
    QList<WordXMLEndnote*> mList;
};

#endif // WORDXMLENDNOTEPROPERTIES_H
