#include "wordxmlstyle.h"

WordXMLStyle::WordXMLStyle()
{
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



    aStream<<"  </w:style>\r\n";
}

void WordXMLStyle::reset()
{
}
