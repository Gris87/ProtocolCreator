#include "wordxmlstyles.h"

WordXMLStyles::WordXMLStyles() : QList<WordXMLStyle>()
{
    reset();
}

WordXMLStyles::~WordXMLStyles()
{
}

void WordXMLStyles::writeToStream(QTextStream &aStream)
{
    aStream<<" <w:styles>\r\n";
    aStream<<"  <w:versionOfBuiltInStylenames w:val=\"4\"/>\r\n";
    aStream<<"  <w:latentStyles w:defLockedState=\"off\" w:latentStyleCount=\"156\"/>\r\n";

    for (int i=0; i<length(); i++)
    {
        aStream<<"\r\n";
        (*this)[i].writeToStream(aStream);
    }

    aStream<<" </w:styles>\r\n";
    aStream<<"\r\n";
}

void WordXMLStyles::reset()
{
    clear();
}
