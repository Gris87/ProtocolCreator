#ifndef WORDXMLSHADING_H
#define WORDXMLSHADING_H

#include "wordxmlbase.h"

#include <QColor>

#include "wordutils.h"

class WordXMLShading : public WordXMLBase
{
public:
    QString pattern;
    QString color;
    QString fillColor;
    QString backgroundColor;

    explicit WordXMLShading(WordXMLBase* aParent=0);
    ~WordXMLShading();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    void setColor(QColor aColor);
    void setFillColor(QColor aColor);
    void setBackgroundColor(QColor aColor);

    WordXMLShading& operator=(const WordXMLShading &another);
};

#endif // WORDXMLSHADING_H
