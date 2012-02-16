#include "wordxmltext.h"

WordXMLText::WordXMLText(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLText::~WordXMLText()
{
}

void WordXMLText::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:t>";
    aStream<<text+"</w:t>\r\n";
}

void WordXMLText::reset()
{
    WordXMLBase::reset();

    text="";
}

bool WordXMLText::isModified()
{
    return text!="";
}
