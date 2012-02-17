#ifndef WORDXMLSECTIONPROPERTIES_H
#define WORDXMLSECTIONPROPERTIES_H

#include "wordxmlbase.h"

#include "wordxmlheaderfooter.h"

class WordXMLSectionProperties : public WordXMLBase
{
public:
    bool needToGenerate;

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

    bool isModified();

    void setPageSize(int aWidth, int aHeight);
    void setPageMargin(int aTop, int aLeft, int aBottom, int aRight, int aHeader=720, int aFooter=720, int aGooter=0);

    WordXMLHeaderFooter* addHeader(EHeaderType aType=htNone);
    WordXMLHeaderFooter* addFooter(EHeaderType aType=htNone);

    WordXMLHeaderFooter* copyHeader(int index);
    WordXMLHeaderFooter* copyHeader(WordXMLHeaderFooter* aHeader);
    WordXMLHeaderFooter* copyFooter(int index);
    WordXMLHeaderFooter* copyFooter(WordXMLHeaderFooter* aHeader);

    WordXMLHeaderFooter* get(int index);

    void remove(int index);
    void remove(WordXMLHeaderFooter* aHeader);

    int count();

    void clear();

private:
    QList<WordXMLHeaderFooter*> mList;
};

#endif // WORDXMLSECTIONPROPERTIES_H
