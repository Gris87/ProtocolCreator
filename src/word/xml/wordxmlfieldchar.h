#ifndef WORDXMLFIELDCHAR_H
#define WORDXMLFIELDCHAR_H

#include "wordxmlbase.h"

enum EFieldCharType
{
    fctBegin,
    fctSeparate,
    fctEnd
};

class WordXMLFieldChar : public WordXMLBase
{
public:
    EFieldCharType type;

    explicit WordXMLFieldChar(WordXMLBase* aParent=0);
    ~WordXMLFieldChar();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();
};

#endif // WORDXMLFIELDCHAR_H
