#include "wordxml.h"

WordXML::WordXML()
{
    reset();
}

WordXML::~WordXML()
{
}

void WordXML::saveToFile(QString aFileName)
{
    QFile aFile(aFileName);
    aFile.open(QIODevice::WriteOnly);

    QTextStream aStream(&aFile);
    aStream.setCodec("UTF-8");

    aStream<<"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n"
             "<?mso-application progid=\"Word.Document\"?>\r\n"

             "<w:wordDocument xmlns:w=\"http://schemas.microsoft.com/office/word/2003/wordml\"\r\n"
             " xmlns:v=\"urn:schemas-microsoft-com:vml\"\r\n"
             " xmlns:w10=\"urn:schemas-microsoft-com:office:word\"\r\n"
             " xmlns:sl=\"http://schemas.microsoft.com/schemaLibrary/2003/core\"\r\n"
             " xmlns:aml=\"http://schemas.microsoft.com/aml/2001/core\"\r\n"
             " xmlns:wx=\"http://schemas.microsoft.com/office/word/2003/auxHint\"\r\n"
             " xmlns:o=\"urn:schemas-microsoft-com:office:office\"\r\n"
             " xmlns:dt=\"uuid:C2F41010-65B3-11d1-A29F-00AA00C14882\"\r\n"
             " xmlns:wsp=\"http://schemas.microsoft.com/office/word/2003/wordml/sp2\"\r\n"
             " w:macrosPresent=\"no\"\r\n"
             " w:embeddedObjPresent=\"no\"\r\n"
             " w:ocxPresent=\"no\"\r\n"
             " xml:space=\"preserve\">\r\n"
             "\r\n"

             " <w:ignoreElements w:val=\"http://schemas.microsoft.com/office/word/2003/wordml/sp2\"/>\r\n"
             "\r\n";

    properties.writeToStream(aStream);
    fonts.writeToStream(aStream);

    aStream<<"</w:wordDocument>\r\n";
}

void WordXML::reset()
{
    properties.reset();
    fonts.reset();
}
