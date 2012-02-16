#include "wordxmlbreak.h"

WordXMLBreak::WordXMLBreak(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLBreak::~WordXMLBreak()
{
}

void WordXMLBreak::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:br/>\r\n";
}
