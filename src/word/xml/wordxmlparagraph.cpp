#include "wordxmlparagraph.h"

WordXMLParagraph::WordXMLParagraph() : WordXMLBase()
{
    reset();
}

WordXMLParagraph::~WordXMLParagraph()
{
}

void WordXMLParagraph::writeToStream(QTextStream &aStream)
{
}

void WordXMLParagraph::reset()
{
}

bool WordXMLParagraph::isModified()
{
    return true;
}
