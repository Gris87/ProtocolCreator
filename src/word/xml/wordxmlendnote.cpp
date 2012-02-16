#include "wordxmlendnote.h"

WordXMLEndnote::WordXMLEndnote() : WordXMLBase()
{
    reset();
}

WordXMLEndnote::~WordXMLEndnote()
{
}

void WordXMLEndnote::writeToStream(QTextStream &aStream)
{
    aStream<<"   <w:endnote w:type=\""+type+"\">\r\n";

    paragraph.writeToStream(aStream);

    aStream<<"   </w:endnote>\r\n";
}

void WordXMLEndnote::reset()
{    
    type="";
    paragraph.reset();
}

bool WordXMLEndnote::isModified()
{
    return type!=""
           ||
           paragraph.isModified();
}
