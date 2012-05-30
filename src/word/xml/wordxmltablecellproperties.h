#ifndef WORDXMLTABLECELLPROPERTIES_H
#define WORDXMLTABLECELLPROPERTIES_H

#include "wordxmlbase.h"

#include "wordxmltablecellmargin.h"
#include "wordxmlshading.h"

enum EMergeType
{
    mtNone,
    mtRestart,
    mtContinue
};

enum ECellAlignment
{
    caNone,
    caTop,
    caCenter,
    caBottom
};

class WordXMLTableCellProperties : public WordXMLBase
{
public:
    int width;
    EMergeType vMergeType;
    int columnSpan;
    ECellAlignment vAlign;
    WordXMLTableCellMargin cellMargin;
    WordXMLShading shading;

    explicit WordXMLTableCellProperties(WordXMLBase* aParent=0);
    ~WordXMLTableCellProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLTableCellProperties& operator=(const WordXMLTableCellProperties &another);
};

#endif // WORDXMLTABLECELLPROPERTIES_H
