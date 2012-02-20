#include "wordxmlsection.h"

WordXMLSection::WordXMLSection(WordXMLBase* aParent) : WordXMLMultiPart(aParent)
{
    componentType=wxtSection;

    properties.parent=this;

    reset();
}

WordXMLSection::~WordXMLSection()
{
}

void WordXMLSection::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<wx:sect>\r\n";

    properties.writeToStream(aStream);
    WordXMLMultiPart::writeToStream(aStream);

    aStream<<space<<"</wx:sect>\r\n";
}

void WordXMLSection::reset()
{
    WordXMLMultiPart::reset();

    properties.reset();
}

bool WordXMLSection::isModified()
{
    return properties.isModified()
           ||
           WordXMLMultiPart::isModified();
}

WordXMLSection& WordXMLSection::operator=(const WordXMLSection &another)
{
    properties=another.properties;

    WordXMLMultiPart *aThis=(WordXMLMultiPart*)this;
    *aThis=another;

    return *this;
}
