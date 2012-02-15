#ifndef WORDXMLPROPERTIES_H
#define WORDXMLPROPERTIES_H

#include <QDateTime>
#include <stdlib.h>

#include "wordxmlbase.h"

#include "wordxmlcustomproperties.h"

class WordXMLProperties : public WordXMLBase
{
public:
    WordXMLCustomProperties customProperties;

    explicit WordXMLProperties();
    ~WordXMLProperties();

    QString title;
    QString subject;
    QString author;
    QString keywords;
    QString description;
    QString lastAuthor;
    QString category;
    QString manager;
    QString company;
    QString hyperlinkBase;

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLPROPERTIES_H