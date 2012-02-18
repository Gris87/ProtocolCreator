#ifndef WORDXMLBREAK_H
#define WORDXMLBREAK_H

#include "wordxmlbase.h"

class WordXMLBreak : public WordXMLBase
{
public:
    explicit WordXMLBreak(WordXMLBase* aParent=0);
    ~WordXMLBreak();

    void writeToStream(QTextStream &aStream);

    WordXMLBreak& operator=(const WordXMLBreak &another);
};

#endif // WORDXMLBREAK_H
