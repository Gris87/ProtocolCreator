#include "wordxmlsectionproperties.h"

WordXMLSectionProperties::WordXMLSectionProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLSectionProperties::~WordXMLSectionProperties()
{
}

void WordXMLSectionProperties::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:sectPr";

    if (rsidR!="")
    {
        aStream<<" wsp:rsidR=\""+rsidR+"\"";
    }

    aStream<<">\r\n";

    aStream<<space<<" <w:pgSz w:w=\""+QString::number(pageSizeWidth)+"\"\r\n"
           <<space<<       "  w:h=\""+QString::number(pageSizeHeight)+"\"/>\r\n";

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

void WordXMLSectionProperties::reset()
{
    WordXMLBase::reset();

    rsidR="";

    pageSizeWidth=11906;
    pageSizeHeight=16838;
    pageMarginTop=1134;
    pageMarginLeft=1701;
    pageMarginBottom=1134;
    pageMarginRight=850;
    pageMarginHeader=720;
    pageMarginFooter=720;
    pageMarginGooter=0;
    colsSpace=720;
    docGridLinePitch=360;
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
