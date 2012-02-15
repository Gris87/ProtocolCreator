#ifndef WORDXMLRUNPROPERTIES_H
#define WORDXMLRUNPROPERTIES_H

#include <QTextStream>

class WordXMLRunProperties
{
public:
    QString font;
    int fontSize;
    int fontSizeCS;
    QString language;
    QString languageFarEast;
    QString languageBIDI;

    explicit WordXMLRunProperties();
    ~WordXMLRunProperties();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLRUNPROPERTIES_H
