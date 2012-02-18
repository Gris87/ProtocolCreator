#ifndef WORDXMLSEPARATOR_H
#define WORDXMLSEPARATOR_H

#include "wordxmlbase.h"

class WordXMLSeparator : public WordXMLBase
{
public:
    explicit WordXMLSeparator(WordXMLBase* aParent=0);
    ~WordXMLSeparator();

    void writeToStream(QTextStream &aStream);

    WordXMLSeparator& operator=(const WordXMLSeparator &another);
};

#endif // WORDXMLSEPARATOR_H
