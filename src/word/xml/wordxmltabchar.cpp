#include "wordxmltabchar.h"

WordXMLTabChar::WordXMLTabChar(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTabChar;

    reset();
}

WordXMLTabChar::~WordXMLTabChar()
{
}

void WordXMLTabChar::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:tab/>\r\n";
}

WordXMLTabChar& WordXMLTabChar::operator=(const WordXMLTabChar &another)
{
    return *this;
}
