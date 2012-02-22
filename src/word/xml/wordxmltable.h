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

    WordXMLTableRow* addRow();

    WordXMLTableRow* copyRow(int index);
    WordXMLTableRow* copyRow(WordXMLTableRow* aRow);

    WordXMLTableRow* getRow(int index);

    void removeRow(int index);
    void removeRow(WordXMLTableRow* aRow);

    int rowCount();

    void clearRows();

    WordXMLTable& operator=(const WordXMLTable &another);

private:
    QList<WordXMLTableRow*> mList;
};

#endif // WORDXMLTABLE_H
