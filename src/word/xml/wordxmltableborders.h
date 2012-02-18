#ifndef WORDXMLTABLEBORDERS_H
#define WORDXMLTABLEBORDERS_H

#include "wordxmlbase.h"

enum ETableBorder
{
    tbNone,
    tbSingle
};

class WordXMLTableBorders : public WordXMLBase
{
public:
    ETableBorder top;
    ETableBorder left;
    ETableBorder bottom;
    ETableBorder right;
    ETableBorder insideH;
    ETableBorder insideV;

    explicit WordXMLTableBorders(WordXMLBase* aParent=0);
    ~WordXMLTableBorders();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLTableBorders& operator=(const WordXMLTableBorders &another);
};

#endif // WORDXMLTABLEBORDERS_H
