#include "wordxmlparagraphproperties.h"

WordXMLParagraphProperties::WordXMLParagraphProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtParagraphProperties;

    borders.parent=this;
    shading.parent=this;
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

        if (
            spacingBefore>=0
            ||
            spacingAfter>=0
            ||
            spacingLine>=0
            ||
            spacingLineRule!=slrNone
           )
        {
            aStream<<space<<" <w:spacing";

            if (spacingBefore>=0)
            {
                aStream<<" w:before=\""+QString::number(spacingBefore)+"\"";
            }

            if (spacingAfter>=0)
            {
                aStream<<" w:after=\""+QString::number(spacingAfter)+"\"";
            }

            if (spacingLine>=0)
            {
                aStream<<" w:line=\""+QString::number(spacingLine)+"\"";
            }

            if (spacingLineRule!=slrNone)
            {
                aStream<<" w:line-rule=\"";

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
                    case slrNone:
                    {
                        // Nothing
                    }
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

        borders.writeToStream(aStream);
        shading.writeToStream(aStream);
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
    spacingBefore=-1;
    spacingAfter=-1;
    spacingLine=-1;
    spacingLineRule=slrNone;
    indentLeft=0;
    indentRight=0;
    indentFirtsLine=0;
    indentHanging=0;
    borders.reset();
    shading.reset();
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
           spacingBefore>=0
           ||
           spacingAfter>=0
           ||
           spacingLine>=0
           ||
           spacingLineRule!=slrNone
           ||
           indentLeft!=0
           ||
           indentRight!=0
           ||
           indentFirtsLine!=0
           ||
           indentHanging!=0
           ||
           borders.isModified()
           ||
           shading.isModified()
           ||
           tabs.isModified()
           ||
           runProperties.isModified()
           ||
           sectionProperties.isModified();
}

void WordXMLParagraphProperties::setFormat(QTextBlockFormat aFormat)
{
    if (aFormat.alignment() & Qt::AlignLeft)
    {
        alignment=paLeft;
    }
    else
    if (aFormat.alignment() & Qt::AlignHCenter)
    {
        alignment=paCenter;
    }
    else
    if (aFormat.alignment() & Qt::AlignRight)
    {
        alignment=paRight;
    }
    else
    if (aFormat.alignment() & Qt::AlignJustify)
    {
        alignment=paBoth;
    }
    else
    {
        alignment=paNone;
    }

    QColor backgroundColor=aFormat.background().color();

    if (aFormat.background().style()!=Qt::NoBrush && backgroundColor.isValid() && backgroundColor!=QColor(255, 255, 255))
    {
        shading.pattern="pct-25";
        shading.setColor(backgroundColor);
        shading.fillColor=shading.color;
        shading.backgroundColor=shading.color;

        /*
        borders.setTopBorder   (btSingle, 6, 15, 1, "auto");
        borders.setLeftBorder  (btSingle, 6, 15, 1, "auto");
        borders.setBottomBorder(btSingle, 6, 15, 1, "auto");
        borders.setRightBorder (btSingle, 6, 15, 1, "auto");
        */
    }
}

WordXMLParagraphProperties& WordXMLParagraphProperties::operator=(const WordXMLParagraphProperties &another)
{
    paragraphStyle=another.paragraphStyle;
    alignment=another.alignment;
    spacingBefore=another.spacingBefore;
    spacingAfter=another.spacingAfter;
    spacingLine=another.spacingLine;
    spacingLineRule=another.spacingLineRule;
    indentLeft=another.indentLeft;
    indentRight=another.indentRight;
    indentFirtsLine=another.indentFirtsLine;
    indentHanging=another.indentHanging;
    borders=another.borders;
    shading=another.shading;
    tabs=another.tabs;
    runProperties=another.runProperties;
    sectionProperties=another.sectionProperties;

    return *this;
}

