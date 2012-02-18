#ifndef WORDXMLTABCHAR_H
#define WORDXMLTABCHAR_H

#include "wordxmlbase.h"

class WordXMLTabChar : public WordXMLBase
{
public:
    explicit WordXMLTabChar(WordXMLBase* aParent=0);
    ~WordXMLTabChar();

    void writeToStream(QTextStream &aStream);

    WordXMLTabChar& operator=(const WordXMLTabChar &another);
};

#endif // WORDXMLTABCHAR_H
