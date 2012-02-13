#include "wordxmlfont.h"

WordXMLFont::WordXMLFont()
{
    reset();
}

WordXMLFont::~WordXMLFont()
{
}

void WordXMLFont::writeToStream(QTextStream &aStream)
{
    aStream<<"  <w:font>\r\n";


    aStream<<"  </w:font>\r\n";
}

void WordXMLFont::reset()
{
}
