#include "wordxmltableproperties.h"

WordXMLTableProperties::WordXMLTableProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
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

        if (indentation>=0)
        {
            aStream<<space<<" <w:tblInd w:w=\""+QString::number(indentation)+"\" w:type=\"dxa\"/>\r\n";
        }

        cellMargin.writeToStream(aStream);

        aStream<<space<<"</w:tblPr>\r\n";
    }
}

void WordXMLTableProperties::reset()
{
    WordXMLBase::reset();

    indentation=-1;
    cellMargin.reset();
}

bool WordXMLTableProperties::isModified()
{
    return indentation>=0
           ||
           cellMargin.isModified();
}
