#include "wordxmlfootnote.h"

WordXMLFootnote::WordXMLFootnote(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtFootnote;

    paragraph.parent=this;

    reset();
}

WordXMLFootnote::~WordXMLFootnote()
{
}

void WordXMLFootnote::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:footnote w:type=\""+type+"\">\r\n";

    paragraph.writeToStream(aStream);

    aStream<<space<<"</w:footnote>\r\n";
}

void WordXMLFootnote::reset()
{    
    WordXMLBase::reset();

    type="";
    paragraph.reset();
}

bool WordXMLFootnote::isModified()
{
    return type!=""
           ||
           paragraph.isModified();
}

WordXMLFootnote& WordXMLFootnote::operator=(const WordXMLFootnote &another)
{
    type=another.type;
    paragraph=another.paragraph;

    return *this;
}
