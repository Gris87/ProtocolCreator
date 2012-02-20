#ifndef WORDXMLPARAGRAPHBORDERS_H
#define WORDXMLPARAGRAPHBORDERS_H

#include "wordxmlbase.h"

#include <QColor>

#include "../wordutils.h"

enum EBorderType
{
    btNone,
    btSingle
};

class WordXMLParagraphBorders : public WordXMLBase
{
public:
    EBorderType topType;
    int topSize;
    int topBorderWidth;
    int topSpace;
    QString topColor;
    EBorderType leftType;
    int leftSize;
    int leftBorderWidth;
    int leftSpace;
    QString leftColor;
    EBorderType bottomType;
    int bottomSize;
    int bottomBorderWidth;
    int bottomSpace;
    QString bottomColor;
    EBorderType rightType;
    int rightSize;
    int rightBorderWidth;
    int rightSpace;
    QString rightColor;

    explicit WordXMLParagraphBorders(WordXMLBase* aParent=0);
    ~WordXMLParagraphBorders();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    void setTopColor(QColor aColor);
    void setLeftColor(QColor aColor);
    void setBottomColor(QColor aColor);
    void setRightColor(QColor aColor);

    void setTopBorder(EBorderType aType, int aSize, int aBorderWidth, int aSpace, QString aColor);
    void setLeftBorder(EBorderType aType, int aSize, int aBorderWidth, int aSpace, QString aColor);
    void setBottomBorder(EBorderType aType, int aSize, int aBorderWidth, int aSpace, QString aColor);
    void setRightBorder(EBorderType aType, int aSize, int aBorderWidth, int aSpace, QString aColor);

    WordXMLParagraphBorders& operator=(const WordXMLParagraphBorders &another);
};

#endif // WORDXMLPARAGRAPHBORDERS_H
