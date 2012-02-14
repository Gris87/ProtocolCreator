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
    aStream<<" <o:DocumentProperties>\r\n";

    if (title!="")
    {
        aStream<<"  <o:Title>"+title+"</o:Title>\r\n";
    }

    if (subject!="")
    {
        aStream<<"  <o:Subject>"+subject+"</o:Subject>\r\n";
    }

    if (author!="")
    {
        aStream<<"  <o:Author>"+author+"</o:Author>\r\n";
    }

    if (keywords!="")
    {
        aStream<<"  <o:Keywords>"+keywords+"</o:Keywords>\r\n";
    }

    if (description!="")
    {
        aStream<<"  <o:Description>"+description+"</o:Description>\r\n";
    }

    if (lastAuthor!="")
    {
        aStream<<"  <o:LastAuthor>"+lastAuthor+"</o:LastAuthor>\r\n";
    }

    if (category!="")
    {
        aStream<<"  <o:Category>"+category+"</o:Category>\r\n";
    }

    if (manager!="")
    {
        aStream<<"  <o:Manager>"+manager+"</o:Manager>\r\n";
    }

    if (company!="")
    {
        aStream<<"  <o:Company>"+company+"</o:Company>\r\n";
    }

    if (hyperlinkBase!="")
    {
        aStream<<"  <o:HyperlinkBase>"+hyperlinkBase+"</o:HyperlinkBase>\r\n";
    }

    aStream<<"  <o:Version>11.0000</o:Version>\r\n";

    customProperties.writeToStream(aStream);

    aStream<<" </o:DocumentProperties>\r\n";
    aStream<<"\r\n";
}

void WordXMLProperties::reset()
{
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
