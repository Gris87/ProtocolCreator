#ifndef     WORDXMLPARAGRAPH_H
#define     WORDXMLPARAGRAPH_H

#include "wordxmlbase.h"

class WordXMLParagraph : public WordXMLBase
{
public:
    explicit WordXMLParagraph();
    ~WordXMLParagraph();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();
};

#endif //     WORDXMLPARAGRAPH_H
