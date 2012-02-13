#include "wordxmlproperties.h"

WordXMLProperties::WordXMLProperties()
{
    reset();
}

WordXMLProperties::~WordXMLProperties()
{
}

void WordXMLProperties::writeToStream(QTextStream &aStream)
{
    lastSaved=QDateTime::currentDateTime();

    if (needToGenerate)
    {
        aStream<<" <o:DocumentProperties>\r\n";
        aStream<<"  <o:Title>"+title+"</o:Title>\r\n";
        aStream<<"  <o:Subject>"+subject+"</o:Subject>\r\n";
        aStream<<"  <o:Author>"+author+"</o:Author>\r\n";
        aStream<<"  <o:Keywords>"+keywords+"</o:Keywords>\r\n";
        aStream<<"  <o:Description>"+description+"</o:Description>\r\n";
        aStream<<"  <o:LastAuthor>"+lastAuthor+"</o:LastAuthor>\r\n";
        aStream<<"  <o:Category>"+category+"</o:Category>\r\n";
        aStream<<"  <o:Manager>"+manager+"</o:Manager>\r\n";
        aStream<<"  <o:Company>"+company+"</o:Company>\r\n";
        aStream<<"  <o:HyperlinkBase>"+hyperlinkBase+"</o:HyperlinkBase>\r\n";
        aStream<<"  <o:Version>11.0000</o:Version>\r\n";

        customProperties.writeToStream(aStream);

        aStream<<" </o:DocumentProperties>\r\n";
    }
}

void WordXMLProperties::reset()
{
    needToGenerate=true;

    title="";
    subject="";
    author=QString(getenv("USERNAME"));
    keywords="";
    description="";
    lastAuthor=author;
    category="";
    manager="";
    company="";
    hyperlinkBase="";

    customProperties.reset();
}
