#include "wordxmlrunproperties.h"

WordXMLRunProperties::WordXMLRunProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLRunProperties::~WordXMLRunProperties()
{
}

void WordXMLRunProperties::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:rPr>\r\n";

        if (font!="")
        {
            aStream<<space<<" <wx:font wx:val=\""+font+"\"/>\r\n";
        }

        if (fontSize>=0)
        {
            aStream<<space<<" <w:sz w:val=\""+QString::number(fontSize*2)+"\"/>\r\n";
        }

        if (fontSizeCS>=0)
        {
            aStream<<space<<" <w:sz-cs w:val=\""+QString::number(fontSizeCS*2)+"\"/>\r\n";
        }

        if (bold!=tsNone)
        {
            if (bold==tsOn)
            {
                aStream<<space<<" <w:b w:val=\"on\"/>\r\n";
            }
            else
            {
                aStream<<space<<" <w:b w:val=\"off\"/>\r\n";
            }
        }

        if (italic!=tsNone)
        {
            if (italic==tsOn)
            {
                aStream<<space<<" <w:i w:val=\"on\"/>\r\n";
            }
            else
            {
                aStream<<space<<" <w:i w:val=\"off\"/>\r\n";
            }
        }

        if (underline!=utNone)
        {
            aStream<<space<<" <w:u w:val=\"";

            switch(underline)
            {
                case utSingle:
                {
                    aStream<<"single";
                }
                break;
                default:
                {
                    throw "unknown underline type";
                }
                break;
            }

            aStream<<"\"/>\r\n";
        }

        if (language!="" || languageFarEast!="" || languageBIDI!="")
        {
            aStream<<space<<" <w:lang";

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

        aStream<<space<<"</w:rPr>\r\n";
    }
}

void WordXMLRunProperties::reset()
{
    WordXMLBase::reset();

    font="";
    fontSize=-1;
    fontSizeCS=-1;
    bold=tsNone;
    italic=tsNone;
    underline=utNone;
    language="";
    languageFarEast="";
    languageBIDI="";
}

bool WordXMLRunProperties::isModified()
{
    return font!=""
           ||
           fontSize>=0
           ||
           fontSizeCS>=0
           ||
           bold!=tsNone
           ||
           italic!=tsNone
           ||
           underline!=utNone
           ||
           language!=""
           ||
           languageFarEast!=""
           ||
           languageBIDI!="";
}
