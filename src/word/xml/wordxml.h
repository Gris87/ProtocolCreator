#ifndef WORDXML_H
#define WORDXML_H

#include <QFile>

#include "wordxmlbase.h"

#include "wordxmlproperties.h"
#include "wordxmlfonts.h"
#include "wordxmlstyles.h"
#include "wordxmldocumentproperties.h"
#include "wordxmlsections.h"

class WordXML : public WordXMLBase
{
public:
    int wordVersion;

    WordXMLProperties properties;
    WordXMLFonts fonts;
    WordXMLStyles styles;
    WordXMLDocumentProperties documentProperties;
    WordXMLSections sections;

    explicit WordXML(int aWordVersion=-1, WordXMLBase* aParent=0);
    ~WordXML();

    void saveToFile(QString aFileName);

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXML& operator=(const WordXML &another);
};

#endif // WORDXML_H
