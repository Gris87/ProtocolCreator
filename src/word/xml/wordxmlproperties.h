#ifndef WORDXMLPROPERTIES_H
#define WORDXMLPROPERTIES_H

#include <QTextStream>
#include <QDateTime>
#include <stdlib.h>

#include "wordxmlcustomproperties.h"

class WordXMLProperties
{
public:
    WordXMLCustomProperties customProperties;

    explicit WordXMLProperties();
    ~WordXMLProperties();

    bool needToGenerate;

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
