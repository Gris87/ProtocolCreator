#ifndef     WORDXMLFOOTNOTE_H
#define     WORDXMLFOOTNOTE_H

#include <QTextStream>

#include "wordxmlparagraph.h"

class WordXMLFootnote
{
public:
    QString type;
    WordXMLParagraph paragraph;

    explicit WordXMLFootnote();
    ~WordXMLFootnote();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif //     WORDXMLFOOTNOTE_H
