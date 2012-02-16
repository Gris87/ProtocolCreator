#include "wordxmltab.h"

WordXMLTab::WordXMLTab() : WordXMLBase()
{
    reset();
}

WordXMLTab::~WordXMLTab()
{
}

void WordXMLTab::writeToStream(QTextStream &aStream)
{
    aStream<<"     <w:tab w:val=\"";

    switch (location)
    {
        case tlLeft:
        {
            aStream<<"left";
        }
        break;
        case tlCenter:
        {
            aStream<<"center";
        }
        break;
        case tlRight:
        {
            aStream<<"right";
        }
        break;
    }

    aStream<<"\" w:pos=\""+QString::number(position)+"\"/>\r\n";
}

void WordXMLTab::reset()
{
    location=tlLeft;
    position=0;
}

bool WordXMLTab::isModified()
{
    return location!=tlLeft
           ||
           position!=0;
}
