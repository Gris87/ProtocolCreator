#ifndef     WORDXMLRUN_H
#define     WORDXMLRUN_H

#include "wordxmlbase.h"

#include "wordxmlrunproperties.h"

class WordXMLRun : public WordXMLBase
{
public:
    WordXMLRunProperties properties;

    explicit WordXMLRun(WordXMLBase* aParent=0);
    ~WordXMLRun();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();
};

#endif //     WORDXMLRUN_H
