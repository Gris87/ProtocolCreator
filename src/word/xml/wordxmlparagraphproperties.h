#ifndef WORDXMLPARAGRAPHPROPERTIES_H
#define WORDXMLPARAGRAPHPROPERTIES_H

#include <QTextStream>

#include "wordxmltabs.h"

class WordXMLParagraphProperties
{
public:
    QString paragraphStyle;
    WordXMLTabs tabs;

    explicit WordXMLParagraphProperties();
    ~WordXMLParagraphProperties();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLPARAGRAPHPROPERTIES_H
