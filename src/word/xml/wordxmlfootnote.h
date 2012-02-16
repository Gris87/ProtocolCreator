#ifndef     WORDXMLFOOTNOTE_H
#define     WORDXMLFOOTNOTE_H

#include "wordxmlbase.h"

#include "wordxmlparagraph.h"

class WordXMLFootnote : public WordXMLBase
{
public:
    QString type;
    WordXMLParagraph paragraph;

    explicit WordXMLFootnote(WordXMLBase* aParent=0);
    ~WordXMLFootnote();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();
};

#endif //     WORDXMLFOOTNOTE_H
