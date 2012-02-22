#ifndef WORDXMLTABLEROW_H
#define WORDXMLTABLEROW_H

#include "wordxmlbase.h"

#include "wordxmltablerowproperties.h"
#include "wordxmltablecell.h"

class WordXMLTableRow : public WordXMLBase
{
public:
    QString rsidR;
    QString rsidTr;
    WordXMLTableRowProperties properties;

    explicit WordXMLTableRow(WordXMLBase* aParent=0);
    ~WordXMLTableRow();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLTableCell* add();

    WordXMLTableCell* copy(int index);
    WordXMLTableCell* copy(WordXMLTableCell* aCell);

    WordXMLTableCell* get(int index);

    void remove(int index);
    void remove(WordXMLTableCell* aCell);

    int count();

    void clear();

    WordXMLTableRow& operator=(const WordXMLTableRow &another);

private:
    QList<WordXMLTableCell*> mList;
};

#endif // WORDXMLTABLEROW_H
