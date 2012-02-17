#ifndef WORDXMLHEADERFOOTER_H
#define WORDXMLHEADERFOOTER_H

#include "wordxmlmultipart.h"

enum EHeaderType
{
    htNone,
    htOdd,
    htEven
};

class WordXMLHeaderFooter : public WordXMLMultiPart
{
public:
    bool isHeader;
    EHeaderType type;

    explicit WordXMLHeaderFooter(WordXMLBase* aParent=0);
    ~WordXMLHeaderFooter();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLHEADERFOOTER_H
