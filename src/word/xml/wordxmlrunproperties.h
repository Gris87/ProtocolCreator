#ifndef WORDXMLRUNPROPERTIES_H
#define WORDXMLRUNPROPERTIES_H

#include "wordxmlbase.h"

class WordXMLRunProperties : public WordXMLBase
{
public:
    QString font;
    int fontSize;
    int fontSizeCS;
    QString language;
    QString languageFarEast;
    QString languageBIDI;

    explicit WordXMLRunProperties(WordXMLBase* aParent=0);
    ~WordXMLRunProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();
};

#endif // WORDXMLRUNPROPERTIES_H
