#ifndef WORDXMLPARAGRAPHPROPERTIES_H
#define WORDXMLPARAGRAPHPROPERTIES_H

#include "wordxmlbase.h"

#include "wordxmltabs.h"
#include "wordxmlrunproperties.h"
#include "wordxmlsectionproperties.h"

class WordXMLParagraphProperties : public WordXMLBase
{
public:
    QString paragraphStyle;
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
