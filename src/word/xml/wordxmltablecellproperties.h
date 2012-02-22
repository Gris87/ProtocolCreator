#ifndef WORDXMLTABLECELLPROPERTIES_H
#define WORDXMLTABLECELLPROPERTIES_H

#include "wordxmlbase.h"

class WordXMLTableCellProperties : public WordXMLBase
{
public:
    explicit WordXMLTableCellProperties(WordXMLBase* aParent=0);
    ~WordXMLTableCellProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLTableCellProperties& operator=(const WordXMLTableCellProperties &another);
};

#endif // WORDXMLTABLECELLPROPERTIES_H
