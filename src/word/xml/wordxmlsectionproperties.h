#ifndef WORDXMLSECTIONPROPERTIES_H
#define WORDXMLSECTIONPROPERTIES_H

#include "wordxmlbase.h"

class WordXMLSectionProperties : public WordXMLBase
{
public:
    QString rsidR;
    int pageSizeWidth;
    int pageSizeHeight;
    int pageMarginTop;
    int pageMarginLeft;
    int pageMarginBottom;
    int pageMarginRight;
    int pageMarginHeader;
    int pageMarginFooter;
    int pageMarginGooter;
    int colsSpace;
    int docGridLinePitch;

    explicit WordXMLSectionProperties(WordXMLBase* aParent=0);
    ~WordXMLSectionProperties();

    void writeToStream(QTextStream &aStream);

    void reset();

    void setPageSize(int aWidth, int aHeight);
    void setPageMargin(int aTop, int aLeft, int aBottom, int aRight, int aHeader=720, int aFooter=720, int aGooter=0);
};

#endif // WORDXMLSECTIONPROPERTIES_H
