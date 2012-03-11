#include "wordxmltablerowproperties.h"

WordXMLTableRowProperties::WordXMLTableRowProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTableRowProperties;

    reset();
}

WordXMLTableRowProperties::~WordXMLTableRowProperties()
{
}

void WordXMLTableRowProperties::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:trPr>\r\n";

        if (height>=0)
        {
            aStream<<space<<" <w:trHeight w:val=\""+QString::number(height)+"\"/>\r\n";
        }

        if (isHeader)
        {
            aStream<<space<<" <w:tblHeader/>\r\n";
        }

        aStream<<space<<"</w:trPr>\r\n";
    }
}

void WordXMLTableRowProperties::reset()
{
    WordXMLBase::reset();

    height=-1;
    isHeader=false;
}

bool WordXMLTableRowProperties::isModified()
{
    return height>=0
           ||
           isHeader;
}

WordXMLTableRowProperties& WordXMLTableRowProperties::operator=(const WordXMLTableRowProperties &another)
{
    height=another.height;
    isHeader=another.isHeader;

    return *this;
}
