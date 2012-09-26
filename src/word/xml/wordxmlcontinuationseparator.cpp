#include "wordxmlcontinuationseparator.h"

WordXMLContinuationSeparator::WordXMLContinuationSeparator(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtContinuationSeparator;

    reset();
}

WordXMLContinuationSeparator::~WordXMLContinuationSeparator()
{
}

void WordXMLContinuationSeparator::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:continuationSeparator/>\r\n";
}

WordXMLContinuationSeparator& WordXMLContinuationSeparator::operator=(const WordXMLContinuationSeparator &/*another*/)
{
    return *this;
}
