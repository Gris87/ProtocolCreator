#include "wordxmlrunproperties.h"

WordXMLRunProperties::WordXMLRunProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtRunProperties;

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
            aStream<<space<<" <w:rFonts w:ascii=\""+font+"\" w:h-ansi=\""+font+"\"/>\r\n";
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

        if (caps)
        {
            aStream<<space<<" <w:caps/>\r\n";
        }

        if (strikeOut)
        {
            aStream<<space<<" <w:strike/>\r\n";
        }

        if (textColor!="")
        {
            aStream<<space<<" <w:color w:val=\""+textColor+"\"/>\r\n";
        }

        if (highlightColor!="")
        {
            aStream<<space<<" <w:highlight w:val=\""+highlightColor+"\"/>\r\n";
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
    caps=false;
    strikeOut=false;
    textColor="";
    highlightColor="";
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
           caps
           ||
           strikeOut
           ||
           textColor!=""
           ||
           highlightColor!=""
           ||
           language!=""
           ||
           languageFarEast!=""
           ||
           languageBIDI!="";
}

void WordXMLRunProperties::setFont(QTextCharFormat aFormat)
{
    setFont(aFormat.font());

    if (aFormat.background().style()!=Qt::NoBrush)
    {
        setHighlightColor(aFormat.background().color());
    }

    if (aFormat.foreground().style()!=Qt::NoBrush)
    {
        setColor(aFormat.foreground().color());
    }
}

void WordXMLRunProperties::setFont(QFont aFont)
{
    font=aFont.family();
    fontSize=aFont.pointSize();

    if (aFont.bold())
    {
        bold=tsOn;
    }
    else
    {
        bold=tsOff;
    }

    if (aFont.italic())
    {
        italic=tsOn;
    }
    else
    {
        italic=tsOff;
    }

    if (aFont.underline())
    {
        underline=utSingle;
    }
    else
    {
        underline=utNone;
    }

    if (aFont.capitalization()==QFont::AllUppercase)
    {
        caps=true;
    }
    else
    {
        caps=false;
    }

    if (aFont.strikeOut())
    {
        strikeOut=true;
    }
    else
    {
        strikeOut=false;
    }
}

void WordXMLRunProperties::setColor(QColor aColor)
{
    textColor=colorToString(aColor);
}

void WordXMLRunProperties::setHighlightColor(QColor aColor)
{
    highlightColor=colorToStringWithRound(aColor);
}

WordXMLRunProperties& WordXMLRunProperties::operator=(const WordXMLRunProperties &another)
{
    font=another.font;
    fontSize=another.fontSize;
    fontSizeCS=another.fontSizeCS;
    bold=another.bold;
    italic=another.italic;
    underline=another.underline;
    caps=another.caps;
    strikeOut=another.strikeOut;
    textColor=another.textColor;
    highlightColor=another.highlightColor;
    language=another.language;
    languageFarEast=another.languageFarEast;
    languageBIDI=another.languageBIDI;

    return *this;
}
