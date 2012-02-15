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
        ||
        fontSizeCS>=0
        ||
        language!=""
        ||
        languageFarEast!=""
        ||
        languageBIDI!=""
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

        if (fontSizeCS>=0)
        {
            aStream<<"    <w:sz-cs w:val=\""+QString::number(fontSizeCS*2)+"\"/>\r\n";
        }

        if (language!="" || languageFarEast!="" || languageBIDI!="")
        {
            aStream<<"    <w:lang";

            if (language!="")
            {
                aStream<<" w:val=\""+language+"\"";
            }

            if (languageFarEast!="")
            {
                aStream<<" w:fareast=\""+languageFarEast+"\"";
            }

            if (languageBIDI!="")
            {
                aStream<<" w:bidi=\""+languageBIDI+"\"";
            }

            aStream<<"/>\r\n";
        }



        aStream<<"   </w:rPr>\r\n";
    }
}

void WordXMLRunProperties::reset()
{
    font="";
    fontSize=-1;
    fontSizeCS=-1;
    language="";
    languageFarEast="";
    languageBIDI="";
}
