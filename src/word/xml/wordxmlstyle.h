#ifndef WORDXMLSTYLE_H
#define WORDXMLSTYLE_H

#include "wordxmlbase.h"

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

class WordXMLStyle : public WordXMLBase
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

    explicit WordXMLStyle(WordXMLBase* aParent=0);
    ~WordXMLStyle();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();
};

#endif // WORDXMLSTYLE_H
