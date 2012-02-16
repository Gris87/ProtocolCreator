#include "wordxmlfonts.h"

WordXMLFonts::WordXMLFonts(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLFonts::~WordXMLFonts()
{
}

void WordXMLFonts::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:fonts>\r\n";
    aStream<<space<<" <w:defaultFonts w:ascii=\""+defaultAsciiFont+"\" w:fareast=\""+defaultFarEastFont+"\" w:h-ansi=\""+defaultHAsciiFont+"\" w:cs=\""+defaultCSFont+"\"/>\r\n";
    aStream<<space<<"</w:fonts>\r\n";
    aStream<<"\r\n";
}

void WordXMLFonts::reset()
{
    WordXMLBase::reset();

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
