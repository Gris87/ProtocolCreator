#include "wordxmltableproperties.h"

WordXMLTableProperties::WordXMLTableProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTableProperties;

    borders.parent=this;
    cellMargin.parent=this;

    reset();
}

WordXMLTableProperties::~WordXMLTableProperties()
{
}

void WordXMLTableProperties::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:tblPr>\r\n";

        if (tableStyle!="")
        {
            aStream<<space<<" <w:tblStyle w:val=\""+tableStyle+"\"/>\r\n";
        }

        if (indentation>=0)
        {
            aStream<<space<<" <w:tblInd w:w=\""+QString::number(indentation)+"\" w:type=\"dxa\"/>\r\n";
        }

        if (alignment!=taNone)
        {
            aStream<<space<<" <w:jc w:val=\"";

            switch(alignment)
            {
                case taLeft:
                {
                    aStream<<"left";
                }
                break;
                case taCenter:
                {
                    aStream<<"center";
                }
                break;
                case taRight:
                {
                    aStream<<"right";
                }
                break;
                case taBoth:
                {
                    aStream<<"both";
                }
                break;
                case taNone:
                {
                    //Nothing
                }
                break;
            }

            aStream<<"\"/>\r\n";
        }

        borders.writeToStream(aStream);
        cellMargin.writeToStream(aStream);

        aStream<<space<<"</w:tblPr>\r\n";
    }
}

void WordXMLTableProperties::reset()
{
    WordXMLBase::reset();

    tableStyle="";
    indentation=-1;
    alignment=taNone;
    borders.reset();
    cellMargin.reset();
}

bool WordXMLTableProperties::isModified()
{
    return tableStyle!=""
           ||
           indentation>=0
           ||
           alignment!=taNone
           ||
           borders.isModified()
           ||
           cellMargin.isModified();
}

WordXMLTableProperties& WordXMLTableProperties::operator=(const WordXMLTableProperties &another)
{
    tableStyle=another.tableStyle;
    indentation=another.indentation;
    alignment=another.alignment;
    borders=another.borders;
    cellMargin=another.cellMargin;

    return *this;
}
