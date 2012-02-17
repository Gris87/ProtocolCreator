#ifndef WORDXMLTABLEPROPERTIES_H
#define WORDXMLTABLEPROPERTIES_H

#include "wordxmlbase.h"

#include "wordxmltablecellmargin.h"
#include "wordxmltableborders.h"

class WordXMLTableProperties : public WordXMLBase
{
public:
    QString tableStyle;
    double indentation;
    WordXMLTableBorders borders;
    WordXMLTableCellMargin cellMargin;

    explicit WordXMLTableProperties(WordXMLBase* aParent=0);
    ~WordXMLTableProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();
};

#endif // WORDXMLTABLEPROPERTIES_H
