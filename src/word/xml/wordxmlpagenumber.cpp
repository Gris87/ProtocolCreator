#include "WordXMLPageNumber.h"

WordXMLPageNumber::WordXMLPageNumber(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLPageNumber::~WordXMLPageNumber()
{
}

void WordXMLPageNumber::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:pgNum/>\r\n";
}
