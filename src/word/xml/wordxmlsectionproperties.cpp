#include "wordxmlsectionproperties.h"

WordXMLSectionProperties::WordXMLSectionProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLSectionProperties::~WordXMLSectionProperties()
{
    clear();
}

void WordXMLSectionProperties::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:sectPr";

        if (rsidR!="")
        {
            aStream<<" wsp:rsidR=\""+rsidR+"\"";
        }

        aStream<<">\r\n";

        for (int i=0; i<mList.length(); i++)
        {
            mList.at(i)->writeToStream(aStream);
        }

        if (landscape)
        {
            aStream<<space<<" <w:pgSz w:w=\""+QString::number(pageSizeHeight)+"\"\r\n"
                   <<space<<       "  w:h=\""+QString::number(pageSizeWidth)+"\"\r\n"
                   <<space<<   "  w:orient=\"landscape\"/>\r\n";
        }
        else
        {
            aStream<<space<<" <w:pgSz w:w=\""+QString::number(pageSizeWidth)+"\"\r\n"
                   <<space<<       "  w:h=\""+QString::number(pageSizeHeight)+"\"/>\r\n";
        }

        aStream<<space<<" <w:pgMar w:top=\""+QString::number(pageMarginTop)+"\"\r\n"
               <<space<<        "  w:left=\""+QString::number(pageMarginLeft)+"\"\r\n"
               <<space<<        "  w:bottom=\""+QString::number(pageMarginBottom)+"\"\r\n"
               <<space<<        "  w:right=\""+QString::number(pageMarginRight)+"\"\r\n"
               <<space<<        "  w:header=\""+QString::number(pageMarginHeader)+"\"\r\n"
               <<space<<        "  w:footer=\""+QString::number(pageMarginFooter)+"\"\r\n"
               <<space<<        "  w:gooter=\""+QString::number(pageMarginGooter)+"\"/>\r\n";

        aStream<<space<<" <w:cols w:space=\""+QString::number(colsSpace)+"\"/>\r\n";

        aStream<<space<<" <w:docGrid w:line-pitch=\""+QString::number(docGridLinePitch)+"\"/>\r\n";

        aStream<<space<<"</w:sectPr>\r\n";
    }
}

void WordXMLSectionProperties::reset()
{
    WordXMLBase::reset();

    needToGenerate=true;

    rsidR="";

    pageSizeWidth=11906;
    pageSizeHeight=16838;
    landscape=false;
    pageMarginTop=1134;
    pageMarginLeft=1701;
    pageMarginBottom=1134;
    pageMarginRight=850;
    pageMarginHeader=720;
    pageMarginFooter=720;
    pageMarginGooter=0;
    colsSpace=720;
    docGridLinePitch=360;

    clear();
}

bool WordXMLSectionProperties::isModified()
{
    return needToGenerate;
}

void WordXMLSectionProperties::setPageSize(int aWidth, int aHeight)
{
    pageSizeWidth=aWidth;
    pageSizeHeight=aHeight;
}

void WordXMLSectionProperties::setPageMargin(int aTop, int aLeft, int aBottom, int aRight, int aHeader, int aFooter, int aGooter)
{
    pageMarginTop=aTop;
    pageMarginLeft=aLeft;
    pageMarginBottom=aBottom;
    pageMarginRight=aRight;
    pageMarginHeader=aHeader;
    pageMarginFooter=aFooter;
    pageMarginGooter=aGooter;
}

WordXMLHeaderFooter* WordXMLSectionProperties::addHeader(EHeaderType aType)
{
    WordXMLHeaderFooter* aNewHeader=new WordXMLHeaderFooter(this);
    aNewHeader->isHeader=true;
    aNewHeader->type=aType;
    mList.append(aNewHeader);
    return aNewHeader;
}

WordXMLHeaderFooter* WordXMLSectionProperties::addFooter(EHeaderType aType)
{
    WordXMLHeaderFooter* aNewHeader=new WordXMLHeaderFooter(this);
    aNewHeader->isHeader=false;
    aNewHeader->type=aType;
    mList.append(aNewHeader);
    return aNewHeader;
}

WordXMLHeaderFooter* WordXMLSectionProperties::copyHeader(int index)
{
    return copyHeader(mList.at(index));
}

WordXMLHeaderFooter* WordXMLSectionProperties::copyHeader(WordXMLHeaderFooter* aHeader)
{
    WordXMLHeaderFooter* aNewHeader=new WordXMLHeaderFooter(this);
    *aNewHeader=*aHeader;
    aNewHeader->isHeader=true;
    mList.append(aNewHeader);
    return aNewHeader;
}

WordXMLHeaderFooter* WordXMLSectionProperties::copyFooter(int index)
{
    return copyFooter(mList.at(index));
}

WordXMLHeaderFooter* WordXMLSectionProperties::copyFooter(WordXMLHeaderFooter* aHeader)
{
    WordXMLHeaderFooter* aNewHeader=new WordXMLHeaderFooter(this);
    *aNewHeader=*aHeader;
    aNewHeader->isHeader=false;
    mList.append(aNewHeader);
    return aNewHeader;
}

WordXMLHeaderFooter* WordXMLSectionProperties::get(int index)
{
    return mList.at(index);
}

void WordXMLSectionProperties::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLSectionProperties::remove(WordXMLHeaderFooter* aHeader)
{
    mList.removeOne(aHeader);
}

int WordXMLSectionProperties::count()
{
    return mList.length();
}

void WordXMLSectionProperties::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}

WordXMLSectionProperties& WordXMLSectionProperties::operator=(const WordXMLSectionProperties &another)
{
    clear();

    for (int i=0; i<another.mList.length(); i++)
    {
        WordXMLHeaderFooter* aNewObject=new WordXMLHeaderFooter(this);
        *aNewObject=*(another.mList.at(i));
        mList.append(aNewObject);
    }

    return *this;
}
