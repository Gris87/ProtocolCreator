#include "wordxmlfootnote.h"

WordXMLFootnote::WordXMLFootnote() : WordXMLBase()
{
    reset();
}

WordXMLFootnote::~WordXMLFootnote()
{
}

void WordXMLFootnote::writeToStream(QTextStream &aStream)
{
    aStream<<"   <w:footnote w:type=\""+type+"\">\r\n";

    paragraph.writeToStream(aStream);

    aStream<<"   </w:footnote>\r\n";
}

void WordXMLFootnote::reset()
{    
    type="";
    paragraph.reset();
}

bool WordXMLFootnote::isModified()
{
    return type!=""
           ||
           paragraph.isModified();
}
