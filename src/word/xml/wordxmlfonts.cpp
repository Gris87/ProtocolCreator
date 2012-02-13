#include "wordxmlfonts.h"

WordXMLFonts::WordXMLFonts() : QList<WordXMLFont>()
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

    for (int i=0; i<length(); i++)
    {
        at(i).writeToStream(aStream);
    }

    aStream<<" </w:fonts>\r\n";
    aStream<<"\r\n";
}

void WordXMLFonts::reset()
{
    defaultAsciiFont="Times New Roman";
    defaultFarEastFont="Times New Roman";
    defaultHAsciiFont="Times New Roman";
    defaultCSFont="Times New Roman";

    clear();
}
