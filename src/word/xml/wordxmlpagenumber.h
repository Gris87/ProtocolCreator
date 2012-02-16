#ifndef WORDXMLPAGENUMBER_H
#define WORDXMLPAGENUMBER_H

#include "wordxmlbase.h"

class WordXMLPageNumber : public WordXMLBase
{
public:
    explicit WordXMLPageNumber(WordXMLBase* aParent=0);
    ~WordXMLPageNumber();

    void writeToStream(QTextStream &aStream);
};

#endif // WORDXMLPAGENUMBER_H
