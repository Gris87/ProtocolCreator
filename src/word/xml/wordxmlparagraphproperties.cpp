#include "wordxmlparagraphproperties.h"

WordXMLParagraphProperties::WordXMLParagraphProperties() : WordXMLBase()
{
    reset();
}

WordXMLParagraphProperties::~WordXMLParagraphProperties()
{
}

void WordXMLParagraphProperties::writeToStream(QTextStream &aStream)
{
    if (
        paragraphStyle!=""
        ||
        tabs.isModified()
       )
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
    paragraphStyle="";
    tabs.reset();
}
