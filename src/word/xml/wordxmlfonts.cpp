#include "wordxmlfonts.h"

WordXMLFonts::WordXMLFonts() : WordXMLBase()
{
    reset();
}

WordXMLFonts::~WordXMLFonts()
{
}

void WordXMLFonts::writeToStream(QTextStream &aStream)
{
    aStream<<" <w:fonts>\r\n";
    aStream<<"  <w:defaultFonts w:ascii=\""+defaultAsciiFont+"\" w:fareast=\""+defaultFarEastFont+"\" w:h-ansi=\""+defaultHAsciiFont+"\" w:cs=\""+defaultCSFont+"\"/>\r\n";
    aStream<<" </w:fonts>\r\n";
    aStream<<"\r\n";
}

void WordXMLFonts::reset()
{
    defaultAsciiFont="Times New Roman";
    defaultFarEastFont="Times New Roman";
    defaultHAsciiFont="Times New Roman";
    defaultCSFont="Times New Roman";
}

bool WordXMLFonts::isModified()
{
    return defaultAsciiFont!="Times New Roman"
           ||
           defaultFarEastFont!="Times New Roman"
           ||
           defaultHAsciiFont!="Times New Roman"
           ||
           defaultCSFont!="Times New Roman";
}
