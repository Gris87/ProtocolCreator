#ifndef WORDXML_H
#define WORDXML_H

#include <QTextStream>
#include <QFile>

#include "wordxmlproperties.h"
#include "wordxmlfonts.h"
#include "wordxmlstyles.h"

class WordXML
{
public:
    WordXMLProperties properties;
    WordXMLFonts fonts;
    WordXMLStyles styles;

    explicit WordXML();
    ~WordXML();

    void saveToFile(QString aFileName);

    void reset();
};

#endif // WORDXML_H
