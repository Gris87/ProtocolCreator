#ifndef WORDXMLTABLEPROPERTIES_H
#define WORDXMLTABLEPROPERTIES_H

#include "wordxmlbase.h"

#include "wordxmltablecellmargin.h"

class WordXMLTableProperties : public WordXMLBase
{
public:
    double indentation;
    WordXMLTableCellMargin cellMargin;

    explicit WordXMLTableProperties(WordXMLBase* aParent=0);
    ~WordXMLTableProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();
};

#endif // WORDXMLTABLEPROPERTIES_H
