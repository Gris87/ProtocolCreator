#include "wordxmlseparator.h"

WordXMLSeparator::WordXMLSeparator(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtSeparator;

    reset();
}

WordXMLSeparator::~WordXMLSeparator()
{
}

void WordXMLSeparator::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:separator/>\r\n";
}

WordXMLSeparator& WordXMLSeparator::operator=(const WordXMLSeparator &/*another*/)
{
    return *this;
}
