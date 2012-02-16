#include "wordxmlparagraphproperties.h"

WordXMLParagraphProperties::WordXMLParagraphProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    tabs.parent=this;
    runProperties.parent=this;
    sectionProperties.parent=this;

    reset();
}

WordXMLParagraphProperties::~WordXMLParagraphProperties()
{
}

void WordXMLParagraphProperties::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:pPr>\r\n";

        if (paragraphStyle!="")
        {
            aStream<<space<<" <w:pStyle w:val=\""+paragraphStyle+"\"/>\r\n";
        }

        tabs.writeToStream(aStream);
        runProperties.writeToStream(aStream);
        sectionProperties.writeToStream(aStream);

        aStream<<space<<"</w:pPr>\r\n";
    }
}

void WordXMLParagraphProperties::reset()
{
    WordXMLBase::reset();

    paragraphStyle="";
    tabs.reset();
    runProperties.reset();
    sectionProperties.reset();
    sectionProperties.needToGenerate=false;
}

bool WordXMLParagraphProperties::isModified()
{
    return paragraphStyle!=""
           ||
           tabs.isModified()
           ||
           runProperties.isModified()
           ||
           sectionProperties.isModified();
}
