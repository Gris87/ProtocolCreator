#ifndef WORDXMLSTYLES_H
#define WORDXMLSTYLES_H

#include <QTextStream>

#include "wordxmlstyle.h"

class WordXMLStyles : public QList<WordXMLStyle>
{
public:
    explicit WordXMLStyles();
    ~WordXMLStyles();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLSTYLES_H
