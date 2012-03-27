#ifndef WORDXMLPARAGRAPHPROPERTIES_H
#define WORDXMLPARAGRAPHPROPERTIES_H

#include "wordxmlbase.h"

#include "wordxmlparagraphborders.h"
#include "wordxmlshading.h"
#include "wordxmltabs.h"
#include "wordxmlrunproperties.h"
#include "wordxmlsectionproperties.h"

enum EParagraphAlignment
{
    paNone,
    paLeft,
    paCenter,
    paRight,
    paBoth
};

enum ESpacingLineRule
{
    slrNone,
    slrAuto,
    slrExact,
    slrAtLeast
};

class WordXMLParagraphProperties : public WordXMLBase
{
public:
    QString paragraphStyle;
    EParagraphAlignment alignment;
    int spacingBefore;
    int spacingAfter;
    int spacingLine;
    ESpacingLineRule spacingLineRule;
    int indentLeft;
    int indentRight;
    int indentFirtsLine;
    int indentHanging;
    WordXMLParagraphBorders borders;
    WordXMLShading shading;
    WordXMLTabs tabs;
    WordXMLRunProperties runProperties;
    WordXMLSectionProperties sectionProperties;

    explicit WordXMLParagraphProperties(WordXMLBase* aParent=0);
    ~WordXMLParagraphProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    void setFormat(QTextBlockFormat aFormat);

    WordXMLParagraphProperties& operator=(const WordXMLParagraphProperties &another);
};

#endif // WORDXMLPARAGRAPHPROPERTIES_H
