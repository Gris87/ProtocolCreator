#include "wordxmlparagraph.h"

WordXMLParagraph::WordXMLParagraph() : WordXMLBase()
{
    reset();
}

WordXMLParagraph::~WordXMLParagraph()
{
}

void WordXMLParagraph::writeToStream(QTextStream &aStream)
{
    aStream<<"   <w:p";

    if (rsidR!="")
    {
        aStream<<" wsp:rsidR=\""+rsidR+"\"";
    }

    if (rsidRPr!="")
    {
        aStream<<" wsp:rsidRPr=\""+rsidRPr+"\"";
    }

    if (rsidRDefault!="")
    {
        aStream<<" wsp:rsidRDefault=\""+rsidRDefault+"\"";
    }

    if (rsidP!="")
    {
        aStream<<" wsp:rsidP=\""+rsidP+"\"";
    }

    aStream<<">\r\n";

    properties.writeToStream(aStream);

    for (int i=0; i<mList.length(); i++)
    {
        mList.at(i)->writeToStream(aStream);
    }

    aStream<<"   </w:p>\r\n";
}

void WordXMLParagraph::reset()
{
    rsidR="";
    rsidRPr="";
    rsidRDefault="";
    rsidP="";
    properties.reset();
}

bool WordXMLParagraph::isModified()
{
    return rsidR!=""
           ||
           rsidRPr!=""
           ||
           rsidRDefault!=""
           ||
           rsidP!=""
           ||
           properties.isModified();
}

WordXMLRun* WordXMLParagraph::addRun()
{
    WordXMLRun* aNewRun=new WordXMLRun();

    mList.append(aNewRun);

    return aNewRun;
}

WordXMLRun* WordXMLParagraph::copyRun(int index)
{
    return copyRun((WordXMLRun*)mList.at(index));
}

WordXMLRun* WordXMLParagraph::copyRun(WordXMLRun* aRun)
{
    WordXMLRun* aNewRun=new WordXMLRun();

    *aNewRun=*aRun;

    mList.append(aNewRun);

    return aNewRun;
}

WordXMLBase* WordXMLParagraph::get(int index)
{
    return mList.at(index);
}

void WordXMLParagraph::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLParagraph::remove(WordXMLBase* aXMLComponent)
{
    mList.removeOne(aXMLComponent);
}

int WordXMLParagraph::count()
{
    return mList.length();
}

void WordXMLParagraph::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}
