#ifndef WORDXML_H
#define WORDXML_H

#include <QFile>

#include "wordxmlbase.h"

#include "wordxmlproperties.h"
#include "wordxmlfonts.h"
#include "wordxmlstyles.h"
#include "wordxmldocumentproperties.h"

class WordXML : public WordXMLBase
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

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXML_H
