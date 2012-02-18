#ifndef WORDXMLTEXT_H
#define WORDXMLTEXT_H

#include "wordxmlbase.h"

class WordXMLText : public WordXMLBase
{
public:
    QString text;

    explicit WordXMLText(WordXMLBase* aParent=0);
    ~WordXMLText();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLText& operator=(const WordXMLText &another);
};

#endif // WORDXMLTEXT_H
