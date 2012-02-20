#include "wordxmlshading.h"

WordXMLShading::WordXMLShading(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtShading;

    reset();
}

WordXMLShading::~WordXMLShading()
{
}

void WordXMLShading::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:shd";

        if (pattern!="")
        {
            aStream<<" w:val=\""+pattern+"\"";
        }

        if (color!="")
        {
            aStream<<" w:color=\""+color+"\"";
        }

        if (fillColor!="")
        {
            aStream<<" w:fill=\""+fillColor+"\"";
        }

        if (backgroundColor!="")
        {
            aStream<<" w:bgcolor=\""+backgroundColor+"\"";
        }

        aStream<<"/>\r\n";
    }
}

void WordXMLShading::reset()
{
    WordXMLBase::reset();

    pattern="";
    color="";
    fillColor="";
    backgroundColor="";
}

bool WordXMLShading::isModified()
{
    return (
            pattern!=""
            ||
            color!=""
            ||
            fillColor!=""
            ||
            backgroundColor!=""
           );
}

void WordXMLShading::setColor(QColor aColor)
{
    color=colorToString(aColor);
}

void WordXMLShading::setFillColor(QColor aColor)
{
    fillColor=colorToString(aColor);
}

void WordXMLShading::setBackgroundColor(QColor aColor)
{
    backgroundColor=colorToString(aColor);
}

WordXMLShading& WordXMLShading::operator=(const WordXMLShading &another)
{
    pattern=another.pattern;
    color=another.color;
    fillColor=another.fillColor;
    backgroundColor=another.backgroundColor;

    return *this;
}
