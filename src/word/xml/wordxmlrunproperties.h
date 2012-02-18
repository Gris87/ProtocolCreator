#ifndef WORDXMLRUNPROPERTIES_H
#define WORDXMLRUNPROPERTIES_H

#include "wordxmlbase.h"

#include "wordxmltypes.h"

enum EUnderlineType
{
    utNone,
    utSingle
};

class WordXMLRunProperties : public WordXMLBase
{
public:
    QString font;
    int fontSize;
    int fontSizeCS;
    ETripleState bold;
    ETripleState italic;
    EUnderlineType underline;
    QString language;
    QString languageFarEast;
    QString languageBIDI;

    explicit WordXMLRunProperties(WordXMLBase* aParent=0);
    ~WordXMLRunProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLRunProperties& operator=(const WordXMLRunProperties &another);
};

#endif // WORDXMLRUNPROPERTIES_H
