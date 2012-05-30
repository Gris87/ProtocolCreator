#include "wordxmltablecellproperties.h"

WordXMLTableCellProperties::WordXMLTableCellProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTableCellProperties;

    cellMargin.parent=this;
    shading.parent=this;

    cellMargin.tagName="tcMar";

    reset();
}

WordXMLTableCellProperties::~WordXMLTableCellProperties()
{
}

void WordXMLTableCellProperties::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:tcPr>\r\n";

        if (width>=0)
        {
            aStream<<space<<" <w:tcW w:w=\""+QString::number(width)+"\" w:type=\"dxa\"/>\r\n";
        }

        if (vMergeType!=mtNone)
        {
            aStream<<space<<" <w:vmerge";

            switch(vMergeType)
            {
                case mtRestart:
                {
                    aStream<<" w:val=\"restart\"";
                }
                break;
                case mtContinue:
                {
                    // Nothing
                }
                break;
                case mtNone:
                {
                    // Nothing
                }
                break;
            }

            aStream<<"/>\r\n";
        }

        if (columnSpan>1)
        {
            aStream<<space<<" <w:gridSpan w:val=\""+QString::number(columnSpan)+"\"/>\r\n";
        }

        if (vAlign!=caNone)
        {
            aStream<<space<<" <w:vAlign w:val=\"";

            switch(vAlign)
            {
                case caTop:
                {
                    aStream<<"top";
                }
                break;
                case caCenter:
                {
                    aStream<<"center";
                }
                break;
                case caBottom:
                {
                    aStream<<"bottom";
                }
                break;
                case caNone:
                {
                    //Nothing
                }
                break;
            }

            aStream<<"\"/>\r\n";
        }

        cellMargin.writeToStream(aStream);
        shading.writeToStream(aStream);

        aStream<<space<<"</w:tcPr>\r\n";
    }
}

void WordXMLTableCellProperties::reset()
{
    WordXMLBase::reset();

    width=-1;
    vMergeType=mtNone;
    columnSpan=1;
    vAlign=caNone;
    cellMargin.reset();
    shading.reset();
}

bool WordXMLTableCellProperties::isModified()
{
    return (
            width>=0
            ||
            vMergeType!=mtNone
            ||
            columnSpan>1
            ||
            vAlign!=caNone
            ||
            cellMargin.isModified()
            ||
            shading.isModified()
           );
}

WordXMLTableCellProperties& WordXMLTableCellProperties::operator=(const WordXMLTableCellProperties &another)
{
    width=another.width;
    vMergeType=another.vMergeType;
    columnSpan=another.columnSpan;
    vAlign=another.vAlign;
    cellMargin=another.cellMargin;
    shading=another.shading;

    return *this;
}
