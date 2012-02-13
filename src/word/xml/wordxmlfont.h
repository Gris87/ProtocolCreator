#ifndef WORDXMLFONT_H
#define WORDXMLFONT_H

#include <QTextStream>

class WordXMLFont
{
public:
    explicit WordXMLFont();
    ~WordXMLFont();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLFONT_H
