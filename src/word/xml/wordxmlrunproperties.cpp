#include "wordxmlrunproperties.h"

WordXMLRunProperties::WordXMLRunProperties()
{
    reset();
}

WordXMLRunProperties::~WordXMLRunProperties()
{
}

void WordXMLRunProperties::writeToStream(QTextStream &aStream)
{
    if (
        font!=""
        ||
        fontSize>=0
       )
    {
        aStream<<"   <w:rPr>\r\n";

        if (font!="")
        {
            aStream<<"    <wx:font wx:val=\""+font+"\"/>\r\n";
        }

        if (fontSize>=0)
        {
            aStream<<"    <w:sz w:val=\""+QString::number(fontSize*2)+"\"/>\r\n";
        }

        aStream<<"   </w:rPr>\r\n";
    }
}

void WordXMLRunProperties::reset()
{
    font="";
    fontSize=-1;
}
