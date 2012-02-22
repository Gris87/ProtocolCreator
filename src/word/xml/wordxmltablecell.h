#ifndef WORDXMLTABLECELL_H
#define WORDXMLTABLECELL_H

#include "wordxmlmultipart.h"

#include "wordxmltablecellproperties.h"

class WordXMLTableCell : public WordXMLMultiPart
{
public:
    WordXMLTableCellProperties properties;

    explicit WordXMLTableCell(WordXMLBase* aParent=0);
    ~WordXMLTableCell();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLTableCell& operator=(const WordXMLTableCell &another);
};

#endif // WORDXMLTABLECELL_H
