#ifndef WORDXMLCONTINUATIONSEPARATOR_H
#define WORDXMLCONTINUATIONSEPARATOR_H

#include "wordxmlbase.h"

class WordXMLContinuationSeparator : public WordXMLBase
{
public:
    explicit WordXMLContinuationSeparator(WordXMLBase* aParent=0);
    ~WordXMLContinuationSeparator();

    void writeToStream(QTextStream &aStream);

    WordXMLContinuationSeparator& operator=(const WordXMLContinuationSeparator &another);
};

#endif // WORDXMLCONTINUATIONSEPARATOR_H
