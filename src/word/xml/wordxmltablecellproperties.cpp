#include "wordxmltablecellproperties.h"

WordXMLTableCellProperties::WordXMLTableCellProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTableCellProperties;

    reset();
}

WordXMLTableCellProperties::~WordXMLTableCellProperties()
{
}

void WordXMLTableCellProperties::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:tcPr>\r\n";

        aStream<<space<<"</w:tcPr>\r\n";
    }
}

void WordXMLTableCellProperties::reset()
{
    WordXMLBase::reset();
}

bool WordXMLTableCellProperties::isModified()
{
    return false;
}

WordXMLTableCellProperties& WordXMLTableCellProperties::operator=(const WordXMLTableCellProperties &another)
{
    return *this;
}
