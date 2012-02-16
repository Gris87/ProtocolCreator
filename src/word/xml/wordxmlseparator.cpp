#include "wordxmlseparator.h"

WordXMLSeparator::WordXMLSeparator(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLSeparator::~WordXMLSeparator()
{
}

void WordXMLSeparator::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:separator/>\r\n";
}
