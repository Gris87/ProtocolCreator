#ifndef WORDXMLTABLEPROPERTIES_H
#define WORDXMLTABLEPROPERTIES_H

#include "wordxmlbase.h"

#include "wordxmltablecellmargin.h"
#include "wordxmltableborders.h"

enum ETableAlignment
{
    taNone,
    taLeft,
    taCenter,
    taRight,
    taBoth
};

class WordXMLTableProperties : public WordXMLBase
{
public:
    QString tableStyle;
    double indentation;
    double width;
    ETableAlignment alignment;
    WordXMLTableBorders borders;
    WordXMLTableCellMargin cellMargin;

    explicit WordXMLTableProperties(WordXMLBase* aParent=0);
    ~WordXMLTableProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLTableProperties& operator=(const WordXMLTableProperties &another);
};

#endif // WORDXMLTABLEPROPERTIES_H
