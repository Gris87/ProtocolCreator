#ifndef WORDXMLRUNPROPERTIES_H
#define WORDXMLRUNPROPERTIES_H

#include "wordxmlbase.h"

#include <QTextCharFormat>

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

    void setFont(QTextCharFormat aFormat);
    void setFont(QFont aFont);

    WordXMLRunProperties& operator=(const WordXMLRunProperties &another);
};

#endif // WORDXMLRUNPROPERTIES_H
