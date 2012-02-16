#include "wordxmlstyle.h"

WordXMLStyle::WordXMLStyle(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    runProperties.parent=this;
    paragraphProperties.parent=this;
    tableProperties.parent=this;

    reset();
}

WordXMLStyle::~WordXMLStyle()
{
}

void WordXMLStyle::writeToStream(QTextStream &aStream)
{
    aStream<<"  <w:style w:type=\"";

    switch (type)
    {
        case stParagraph:
        {
            aStream<<"paragraph";
        }
        break;
        case stCharacter:
        {
            aStream<<"character";
        }
        break;
        case stTable:
        {
            aStream<<"table";
        }
        break;
        case stList:
        {
            aStream<<"list";
        }
        break;
    }

    aStream<<"\" w:default=\"";

    if (isDefault)
    {
        aStream<<"1";
    }
    else
    {
        aStream<<"0";
    }

    aStream<<"\" w:styleId=\""+styleID+"\">\r\n";

    if (name!="")
    {
        aStream<<"   <w:name w:val=\""+name+"\"/>\r\n";
    }

    if (uiName!="")
    {
        aStream<<"   <w:uiName w:val=\""+uiName+"\"/>\r\n";
    }

    if (basedOn!="")
    {
        aStream<<"   <w:basedOn w:val=\""+basedOn+"\"/>\r\n";
    }

    if (rsid!="")
    {
        aStream<<"   <w:rsid w:val=\""+rsid+"\"/>\r\n";
    }

    runProperties.writeToStream(aStream);
    paragraphProperties.writeToStream(aStream);
    tableProperties.writeToStream(aStream);

    if (isSemiHidden)
    {
        aStream<<"   <w:semiHidden/>\r\n";
    }

    aStream<<"  </w:style>\r\n";
}

void WordXMLStyle::reset()
{
    WordXMLBase::reset();

    type=stParagraph;
    isDefault=true;
    styleID="";
    name="";
    uiName="";
    basedOn="";
    rsid="";

    runProperties.reset();
    paragraphProperties.reset();
    tableProperties.reset();

    isSemiHidden=false;
}

bool WordXMLStyle::isModified()
{
    return type!=stParagraph
           ||
           !isDefault
           ||
           styleID!=""
           ||
           name!=""
           ||
           uiName!=""
           ||
           basedOn!=""
           ||
           rsid!=""
           ||
           runProperties.isModified()
           ||
           paragraphProperties.isModified()
           ||
           tableProperties.isModified()
           ||
           isSemiHidden;
}
