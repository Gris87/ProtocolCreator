#include "wordxmltab.h"

WordXMLTab::WordXMLTab(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTab;

    reset();
}

WordXMLTab::~WordXMLTab()
{
}

void WordXMLTab::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:tab w:val=\"";

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
    WordXMLBase::reset();

    location=tlLeft;
    position=0;
}

bool WordXMLTab::isModified()
{
    return location!=tlLeft
           ||
           position!=0;
}

WordXMLTab& WordXMLTab::operator=(const WordXMLTab &another)
{
    location=another.location;
    position=another.position;

    return *this;
}

