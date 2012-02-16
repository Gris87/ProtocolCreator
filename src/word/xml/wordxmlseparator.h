#ifndef WORDXMLSEPARATOR_H
#define WORDXMLSEPARATOR_H

#include "wordxmlbase.h"

class WordXMLSeparator : public WordXMLBase
{
public:
    explicit WordXMLSeparator(WordXMLBase* aParent=0);
    ~WordXMLSeparator();

    void writeToStream(QTextStream &aStream);
};

#endif // WORDXMLSEPARATOR_H
