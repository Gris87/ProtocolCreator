#ifndef WORDXMLTABLE_H
#define WORDXMLTABLE_H

#include "wordxmlbase.h"

#include "wordxmltableproperties.h"
#include "wordxmltablerow.h"

class WordXMLTable : public WordXMLBase
{
public:
    WordXMLTableProperties properties;

    explicit WordXMLTable(WordXMLBase* aParent=0);
    ~WordXMLTable();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLTableRow* add();

    WordXMLTableRow* copy(int index);
    WordXMLTableRow* copy(WordXMLTableRow* aRow);

    WordXMLTableRow* get(int index);

    void remove(int index);
    void remove(WordXMLTableRow* aRow);

    int count();

    void clear();

    WordXMLTable& operator=(const WordXMLTable &another);

private:
    QList<WordXMLTableRow*> mList;
};

#endif // WORDXMLTABLE_H
