#ifndef WORDXMLPARAGRAPHPROPERTIES_H
#define WORDXMLPARAGRAPHPROPERTIES_H

#include "wordxmlbase.h"

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
    slrAuto,
    slrExact,
    slrAtLeast
};

class WordXMLParagraphProperties : public WordXMLBase
{
public:
    QString paragraphStyle;
    EParagraphAlignment alignment;
    int spacingLine;
    ESpacingLineRule spacingLineRule;
    int indentLeft;
    int indentRight;
    int indentFirtsLine;
    int indentHanging;
    WordXMLTabs tabs;
    WordXMLRunProperties runProperties;
    WordXMLSectionProperties sectionProperties;

    explicit WordXMLParagraphProperties(WordXMLBase* aParent=0);
    ~WordXMLParagraphProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();
};

#endif // WORDXMLPARAGRAPHPROPERTIES_H
