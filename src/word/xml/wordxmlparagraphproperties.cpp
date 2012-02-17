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

        if (alignment!=paNone)
        {
            aStream<<space<<" <w:jc w:val=\"";

            switch(alignment)
            {
                case paLeft:
                {
                    aStream<<"left";
                }
                break;
                case paCenter:
                {
                    aStream<<"center";
                }
                break;
                case paRight:
                {
                    aStream<<"right";
                }
                break;
                case paBoth:
                {
                    aStream<<"both";
                }
                break;
                case paNone:
                {
                    //Nothing
                }
                break;
            }

            aStream<<"\"/>\r\n";
        }

        if (spacingLine>=0)
        {
            aStream<<space<<" <w:spacing w:line=\""+QString::number(spacingLine)+"\" w:line-rule=\"";

            switch(spacingLineRule)
            {
                case slrAuto:
                {
                    aStream<<"left";
                }
                break;
                case slrExact:
                {
                    aStream<<"exact";
                }
                break;
                case slrAtLeast:
                {
                    aStream<<"at-least";
                }
            }

            aStream<<"\"/>\r\n";
        }

        if (
            indentLeft!=0
            ||
            indentRight!=0
            ||
            indentFirtsLine!=0
            ||
            indentHanging!=0
           )
        {
            aStream<<space<<" <w:ind";

            if (indentLeft!=0)
            {
                aStream<<" w:left=\""+QString::number(indentLeft)+"\"";
            }

            if (indentRight!=0)
            {
                aStream<<" w:right=\""+QString::number(indentRight)+"\"";
            }

            if (indentFirtsLine!=0 && indentHanging==0)
            {
                aStream<<" w:first-line=\""+QString::number(indentFirtsLine)+"\"";
            }

            if (indentHanging!=0)
            {
                aStream<<" w:hanging=\""+QString::number(indentHanging)+"\"";
            }

            aStream<<"/>\r\n";
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
    alignment=paNone;
    spacingLine=-1;
    spacingLineRule=slrAuto;
    indentLeft=0;
    indentRight=0;
    indentFirtsLine=0;
    indentHanging=0;
    tabs.reset();
    runProperties.reset();
    sectionProperties.reset();
    sectionProperties.needToGenerate=false;
}

bool WordXMLParagraphProperties::isModified()
{
    return paragraphStyle!=""
           ||
           alignment!=paNone
           ||
           spacingLine>=0
           ||
           indentLeft!=0
           ||
           indentRight!=0
           ||
           indentFirtsLine!=0
           ||
           indentHanging!=0
           ||
           tabs.isModified()
           ||
           runProperties.isModified()
           ||
           sectionProperties.isModified();
}
