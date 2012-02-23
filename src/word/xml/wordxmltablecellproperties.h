#ifndef WORDXMLTABLECELLPROPERTIES_H
#define WORDXMLTABLECELLPROPERTIES_H

#include "wordxmlbase.h"

#include "wordxmlshading.h"

enum EMergeType
{
    mtNone,
    mtRestart,
    mtContinue
};

class WordXMLTableCellProperties : public WordXMLBase
{
public:
    int width;
    EMergeType vMergeType;
    int columnSpan;
    WordXMLShading shading;

    explicit WordXMLTableCellProperties(WordXMLBase* aParent=0);
    ~WordXMLTableCellProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLTableCellProperties& operator=(const WordXMLTableCellProperties &another);
};

#endif // WORDXMLTABLECELLPROPERTIES_H
