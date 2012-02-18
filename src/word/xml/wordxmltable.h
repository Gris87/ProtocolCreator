#ifndef WORDXMLTABLE_H
#define WORDXMLTABLE_H

#include "wordxmlbase.h"

class WordXMLTable : public WordXMLBase
{
public:
    explicit WordXMLTable(WordXMLBase* aParent=0);
    ~WordXMLTable();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLTable& operator=(const WordXMLTable &another);
};

#endif // WORDXMLTABLE_H
