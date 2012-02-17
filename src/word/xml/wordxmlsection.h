#ifndef WORDXMLSECTION_H
#define WORDXMLSECTION_H

#include "wordxmlmultipart.h"

#include "wordxmlsectionproperties.h"

class WordXMLSection : public WordXMLMultiPart
{
public:
    WordXMLSectionProperties properties;

    explicit WordXMLSection(WordXMLBase* aParent=0);
    ~WordXMLSection();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();
};

#endif // WORDXMLSECTION_H
