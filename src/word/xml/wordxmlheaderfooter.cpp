#include "wordxmlheaderfooter.h"

WordXMLHeaderFooter::WordXMLHeaderFooter(WordXMLBase* aParent) : WordXMLMultiPart(aParent)
{
    componentType=wxtHeaderFooter;

    isHeader=true;

    reset();
}

WordXMLHeaderFooter::~WordXMLHeaderFooter()
{
}

void WordXMLHeaderFooter::writeToStream(QTextStream &aStream)
{
    if (isHeader)
    {
        aStream<<space<<"<w:hdr";
    }
    else
    {
        aStream<<space<<"<w:ftr";
    }

    switch (type)
    {
        case htOdd:
        {
            aStream<<" w:type=\"odd\"";
        }
        break;
        case htEven:
        {
            aStream<<" w:type=\"even\"";
        }
        break;
        case htNone:
        {
            //Nothing
        }
        break;
    }

    aStream<<">\r\n";

    WordXMLMultiPart::writeToStream(aStream);

    if (isHeader)
    {
        aStream<<space<<"</w:hdr>";
    }
    else
    {
        aStream<<space<<"</w:ftr>";
    }
}

void WordXMLHeaderFooter::reset()
{
    WordXMLMultiPart::reset();

    type=htNone;
}

WordXMLHeaderFooter& WordXMLHeaderFooter::operator=(const WordXMLHeaderFooter &another)
{
    isHeader=another.isHeader;
    type=another.type;

    return *this;
}
