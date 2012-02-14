#ifndef WORDXMLSTYLE_H
#define WORDXMLSTYLE_H

#include <QTextStream>

#include "wordxmlrunproperties.h"
#include "wordxmlparagraphproperties.h"
#include "wordxmltableproperties.h"

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
    QString name;
    QString uiName;
    QString basedOn;
    QString rsid;
    WordXMLRunProperties runProperties;
    WordXMLParagraphProperties paragraphProperties;
    WordXMLTableProperties tableProperties;
    bool isSemiHidden;

    explicit WordXMLStyle();
    ~WordXMLStyle();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLSTYLE_H
