#ifndef     WORDXMLPARAGRAPH_H
#define     WORDXMLPARAGRAPH_H

#include <QTextStream>

class WordXMLParagraph
{
public:
    explicit WordXMLParagraph();
    ~WordXMLParagraph();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif //     WORDXMLPARAGRAPH_H
