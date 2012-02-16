#include "wordxmltabchar.h"

WordXMLTabChar::WordXMLTabChar(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLTabChar::~WordXMLTabChar()
{
}

void WordXMLTabChar::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:tab/>\r\n";
}
