#ifndef     WORDXMLENDNOTE_H
#define     WORDXMLENDNOTE_H

#include <QTextStream>

#include "wordxmlparagraph.h"

class WordXMLEndnote
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
