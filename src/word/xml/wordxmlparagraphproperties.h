#ifndef WORDXMLPARAGRAPHPROPERTIES_H
#define WORDXMLPARAGRAPHPROPERTIES_H

#include "wordxmlbase.h"

#include "wordxmltabs.h"

class WordXMLParagraphProperties : public WordXMLBase
{
public:
    QString paragraphStyle;
    WordXMLTabs tabs;

    explicit WordXMLParagraphProperties(WordXMLBase* aParent=0);
    ~WordXMLParagraphProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();
};

#endif // WORDXMLPARAGRAPHPROPERTIES_H
