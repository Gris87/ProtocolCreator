#include "wordxmlproperties.h"

WordXMLProperties::WordXMLProperties(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtProperties;

    customProperties.parent=this;

    reset();
}

WordXMLProperties::~WordXMLProperties()
{
}

void WordXMLProperties::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<o:DocumentProperties>\r\n";

    if (title!="")
    {
        aStream<<space<<" <o:Title>"+title+"</o:Title>\r\n";
    }

    if (subject!="")
    {
        aStream<<space<<" <o:Subject>"+subject+"</o:Subject>\r\n";
    }

    if (author!="")
    {
        aStream<<space<<" <o:Author>"+author+"</o:Author>\r\n";
    }

    if (keywords!="")
    {
        aStream<<space<<" <o:Keywords>"+keywords+"</o:Keywords>\r\n";
    }

    if (description!="")
    {
        aStream<<space<<" <o:Description>"+description+"</o:Description>\r\n";
    }

    if (lastAuthor!="")
    {
        aStream<<space<<" <o:LastAuthor>"+lastAuthor+"</o:LastAuthor>\r\n";
    }

    if (category!="")
    {
        aStream<<space<<" <o:Category>"+category+"</o:Category>\r\n";
    }

    if (manager!="")
    {
        aStream<<space<<" <o:Manager>"+manager+"</o:Manager>\r\n";
    }

    if (company!="")
    {
        aStream<<space<<" <o:Company>"+company+"</o:Company>\r\n";
    }

    if (hyperlinkBase!="")
    {
        aStream<<space<<" <o:HyperlinkBase>"+hyperlinkBase+"</o:HyperlinkBase>\r\n";
    }

    aStream<<space<<" <o:Version>11.0000</o:Version>\r\n";

    customProperties.writeToStream(aStream);

    aStream<<space<<"</o:DocumentProperties>\r\n";
    aStream<<"\r\n";
}

void WordXMLProperties::reset()
{
    WordXMLBase::reset();

    title="";
    subject="";
    author=QString::fromUtf8(qgetenv("USERNAME"));
    keywords="";
    description="";
    lastAuthor=author;
    category="";
    manager="";
    company="";
    hyperlinkBase="";

    customProperties.reset();
}

bool WordXMLProperties::isModified()
{
    return title!=""
           ||
           subject!=""
           ||
           author!=QString::fromUtf8(qgetenv("USERNAME"))
           ||
           keywords!=""
           ||
           description!=""
           ||
           lastAuthor!=author
           ||
           category!=""
           ||
           manager!=""
           ||
           company!=""
           ||
           hyperlinkBase!=""
           ||
           customProperties.isModified();
}

WordXMLProperties& WordXMLProperties::operator=(const WordXMLProperties &another)
{
    title=another.title;
    subject=another.subject;
    author=another.author;
    keywords=another.keywords;
    description=another.description;
    lastAuthor=another.lastAuthor;
    category=another.category;
    manager=another.manager;
    company=another.company;
    hyperlinkBase=another.hyperlinkBase;
    customProperties=another.customProperties;

    return *this;
}
