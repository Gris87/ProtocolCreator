#ifndef WORDXMLFOOTNOTEPROPERTIES_H
#define WORDXMLFOOTNOTEPROPERTIES_H

#include "wordxmlbase.h"

#include "wordxmlfootnote.h"

class WordXMLFootnoteProperties : public WordXMLBase
{
public:
    explicit WordXMLFootnoteProperties(WordXMLBase* aParent=0);
    ~WordXMLFootnoteProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

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

    void clear();

private:
    QList<WordXMLFootnote*> mList;
};

#endif // WORDXMLFOOTNOTEPROPERTIES_H
