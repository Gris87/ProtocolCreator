#include "wordxmlparagraphproperties.h"

WordXMLParagraphProperties::WordXMLParagraphProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    tabs.parent=this;

    reset();
}

WordXMLParagraphProperties::~WordXMLParagraphProperties()
{
}

void WordXMLParagraphProperties::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<"   <w:pPr>\r\n";

        if (paragraphStyle!="")
        {
            aStream<<"    <w:pStyle w:val=\""+paragraphStyle+"\"/>\r\n";
        }

        tabs.writeToStream(aStream);

        aStream<<"   </w:pPr>\r\n";
    }
}

void WordXMLParagraphProperties::reset()
{
    WordXMLBase::reset();

    paragraphStyle="";
    tabs.reset();
}

bool WordXMLParagraphProperties::isModified()
{
    return paragraphStyle!=""
           ||
           tabs.isModified();
}
