#ifndef WORDXML_H
#define WORDXML_H

#include <QTextStream>
#include <QFile>

#include "wordxmlproperties.h"
#include "wordxmlfonts.h"
#include "wordxmlstyles.h"
#include "wordxmldocumentproperties.h"

class WordXML
{
public:
    int wordVersion;

    WordXMLProperties properties;
    WordXMLFonts fonts;
    WordXMLStyles styles;
    WordXMLDocumentProperties documentProperties;

    explicit WordXML(int aWordVersion=-1);
    ~WordXML();

    void saveToFile(QString aFileName);

    void reset();
};

#endif // WORDXML_H
