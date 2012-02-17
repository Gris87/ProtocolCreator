#include "wordxmltableproperties.h"

WordXMLTableProperties::WordXMLTableProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
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
    borders.reset();
    cellMargin.reset();
}

bool WordXMLTableProperties::isModified()
{
    return tableStyle!=""
           ||
           indentation>=0
           ||
           borders.isModified()
           ||
           cellMargin.isModified();
}
