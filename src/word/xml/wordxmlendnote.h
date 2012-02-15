#ifndef     WORDXMLENDNOTE_H
#define     WORDXMLENDNOTE_H

#include "wordxmlbase.h"

#include "wordxmlparagraph.h"

class WordXMLEndnote : public WordXMLBase
{
public:
    QString type;
    WordXMLParagraph paragraph;

    explicit WordXMLEndnote();
    ~WordXMLEndnote();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif //     WORDXMLENDNOTE_H
