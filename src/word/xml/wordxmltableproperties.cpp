#include "wordxmltableproperties.h"

WordXMLTableProperties::WordXMLTableProperties() : WordXMLBase()
{
    reset();
}

WordXMLTableProperties::~WordXMLTableProperties()
{
}

void WordXMLTableProperties::writeToStream(QTextStream &aStream)
{
    if (
        indentation>=0
        ||
        cellMargin.isModified()
       )
    {
        aStream<<"   <w:tblPr>\r\n";

        if (indentation>=0)
        {
            aStream<<"    <w:tblInd w:w=\""+QString::number(indentation)+"\" w:type=\"dxa\"/>\r\n";
        }

        cellMargin.writeToStream(aStream);

        aStream<<"   </w:tblPr>\r\n";
    }
}

void WordXMLTableProperties::reset()
{
    indentation=-1;
    cellMargin.reset();
}
