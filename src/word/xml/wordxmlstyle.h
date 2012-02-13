#ifndef WORDXMLSTYLE_H
#define WORDXMLSTYLE_H

#include <QTextStream>

enum EStyleType
{
    stParagraph,
    stCharacter,
    stTable,
    stList
};


class WordXMLStyle
{
public:
    EStyleType type;
    bool isDefault;
    QString styleID;

    explicit WordXMLStyle();
    ~WordXMLStyle();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLSTYLE_H
