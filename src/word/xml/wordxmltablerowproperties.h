#ifndef WORDXMLTABLEROWPROPERTIES_H
#define WORDXMLTABLEROWPROPERTIES_H

#include "wordxmlbase.h"

class WordXMLTableRowProperties : public WordXMLBase
{
public:
    int height;
    bool isHeader;

    explicit WordXMLTableRowProperties(WordXMLBase* aParent=0);
    ~WordXMLTableRowProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLTableRowProperties& operator=(const WordXMLTableRowProperties &another);
};

#endif // WORDXMLTABLEROWPROPERTIES_H
