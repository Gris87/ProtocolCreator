#ifndef WORDXMLPARAGRAPHPROPERTIES_H
#define WORDXMLPARAGRAPHPROPERTIES_H

#include <QTextStream>

class WordXMLParagraphProperties
{
public:
    explicit WordXMLParagraphProperties();
    ~WordXMLParagraphProperties();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLPARAGRAPHPROPERTIES_H
