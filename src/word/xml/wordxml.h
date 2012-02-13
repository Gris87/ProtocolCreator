#ifndef WORDXML_H
#define WORDXML_H

#include <QTextStream>
#include <QFile>

#include "wordxmlproperties.h"
#include "wordxmlfonts.h"

class WordXML
{
public:
    WordXMLProperties properties;
    WordXMLFonts fonts;

    explicit WordXML();
    ~WordXML();

    void saveToFile(QString aFileName);

    void reset();
};

#endif // WORDXML_H
