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

        aStream<<space<<"</w:trPr>\r\n";
    }
}

void WordXMLTableRowProperties::reset()
{
    WordXMLBase::reset();
}

bool WordXMLTableRowProperties::isModified()
{
    return false;
}

WordXMLTableRowProperties& WordXMLTableRowProperties::operator=(const WordXMLTableRowProperties &another)
{
    return *this;
}
