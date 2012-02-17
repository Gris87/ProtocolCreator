#ifndef WORDXMLHEADERFOOTER_H
#define WORDXMLHEADERFOOTER_H

#include "wordxmlbase.h"

//#include "wordxmlparagraph.h"
//#include "wordxmltable.h"

class WordXMLParagraph;
class WordXMLTable;

enum EHeaderType
{
    htNone,
    htOdd,
    htEven
};

class WordXMLHeaderFooter : public WordXMLBase
{
public:
    bool isHeader;
    EHeaderType type;

    explicit WordXMLHeaderFooter(WordXMLBase* aParent=0);
    ~WordXMLHeaderFooter();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLParagraph* addParagraph();
    WordXMLTable* addTable();

    WordXMLBase* get(int index);

    void remove(int index);
    void remove(WordXMLBase* aXMLComponent);

    int count();

    void clear();

private:
    QList<WordXMLBase*> mList;
};

#endif // WORDXMLHEADERFOOTER_H
