#ifndef WORDXMLPROPERTIES_H
#define WORDXMLPROPERTIES_H

#include <QDateTime>
#include <stdlib.h>

#include "wordxmlbase.h"

#include "wordxmlcustomproperties.h"

class WordXMLProperties : public WordXMLBase
{
public:
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
    WordXMLCustomProperties customProperties;

    explicit WordXMLProperties(WordXMLBase* aParent=0);
    ~WordXMLProperties();    

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    WordXMLProperties& operator=(const WordXMLProperties &another);
};

#endif // WORDXMLPROPERTIES_H
