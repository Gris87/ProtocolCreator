#include "wordxmlparagraph.h"

WordXMLParagraph::WordXMLParagraph(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    properties.parent=this;

    reset();
}

WordXMLParagraph::~WordXMLParagraph()
{
    clear();
}

void WordXMLParagraph::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:p";

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

    aStream<<space<<"</w:p>\r\n";
}

void WordXMLParagraph::reset()
{
    WordXMLBase::reset();

    rsidR="";
    rsidRPr="";
    rsidRDefault="";
    rsidP="";
    properties.reset();

    clear();
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
           properties.isModified()
           ||
           mList.length()>0;
}

WordXMLRun* WordXMLParagraph::addRun()
{
    WordXMLRun* aNewRun=new WordXMLRun(this);

    mList.append(aNewRun);

    return aNewRun;
}

WordXMLRun* WordXMLParagraph::copyRun(int index)
{
    return copyRun((WordXMLRun*)mList.at(index));
}

WordXMLRun* WordXMLParagraph::copyRun(WordXMLRun* aRun)
{
    WordXMLRun* aNewRun=new WordXMLRun(this);

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
