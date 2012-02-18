#include "wordxmlbreak.h"

WordXMLBreak::WordXMLBreak(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtBreak;

    reset();
}

WordXMLBreak::~WordXMLBreak()
{
}

void WordXMLBreak::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:br/>\r\n";
}

WordXMLBreak& WordXMLBreak::operator=(const WordXMLBreak &another)
{
    return *this;
}
