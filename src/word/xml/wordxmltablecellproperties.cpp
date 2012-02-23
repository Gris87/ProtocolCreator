#include "wordxmltablecellproperties.h"

WordXMLTableCellProperties::WordXMLTableCellProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTableCellProperties;

    shading.parent=this;

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
            shading.isModified()
           );
}

WordXMLTableCellProperties& WordXMLTableCellProperties::operator=(const WordXMLTableCellProperties &another)
{
    width=another.width;
    vMergeType=another.vMergeType;
    columnSpan=another.columnSpan;
    shading=another.shading;

    return *this;
}
