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

    WordXMLTableCell* addCell();

    WordXMLTableCell* copyCell(int index);
    WordXMLTableCell* copyCell(WordXMLTableCell* aCell);

    WordXMLTableCell* getCell(int index);

    void removeCell(int index);
    void removeCell(WordXMLTableCell* aCell);

    int cellCount();

    void clearCells();

    WordXMLTableRow& operator=(const WordXMLTableRow &another);

private:
    QList<WordXMLTableCell*> mList;
};

#endif // WORDXMLTABLEROW_H
