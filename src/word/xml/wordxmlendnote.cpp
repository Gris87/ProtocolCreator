#include "wordxmlendnote.h"

WordXMLEndnote::WordXMLEndnote(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtEndnote;

    paragraph.parent=this;

    reset();
}

WordXMLEndnote::~WordXMLEndnote()
{
}

void WordXMLEndnote::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:endnote w:type=\""+type+"\">\r\n";

    paragraph.writeToStream(aStream);

    aStream<<space<<"</w:endnote>\r\n";
}

void WordXMLEndnote::reset()
{    
    WordXMLBase::reset();

    type="";
    paragraph.reset();
}

bool WordXMLEndnote::isModified()
{
    return type!=""
           ||
           paragraph.isModified();
}

WordXMLEndnote& WordXMLEndnote::operator=(const WordXMLEndnote &another)
{
    type=another.type;
    paragraph=another.paragraph;

    return *this;
}
