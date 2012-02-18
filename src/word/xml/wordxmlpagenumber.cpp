#include "WordXMLPageNumber.h"

WordXMLPageNumber::WordXMLPageNumber(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtPageNumber;

    reset();
}

WordXMLPageNumber::~WordXMLPageNumber()
{
}

void WordXMLPageNumber::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:pgNum/>\r\n";
}

WordXMLPageNumber& WordXMLPageNumber::operator=(const WordXMLPageNumber &another)
{
    return *this;
}
