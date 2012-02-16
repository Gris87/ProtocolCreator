#include "wordxmlrun.h"

WordXMLRun::WordXMLRun(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    properties.parent=this;

    reset();
}

WordXMLRun::~WordXMLRun()
{
    clear();
}

void WordXMLRun::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:r";

    if (rsidR!="")
    {
        aStream<<" wsp:rsidR=\""+rsidR+"\"";
    }

    aStream<<">\r\n";

    properties.writeToStream(aStream);

    for (int i=0; i<mList.length(); i++)
    {
        mList.at(i)->writeToStream(aStream);
    }

    aStream<<space<<"</w:r>\r\n";
}

void WordXMLRun::reset()
{
    WordXMLBase::reset();

    rsidR="";
    properties.reset();

    clear();
}

bool WordXMLRun::isModified()
{
    return rsidR!=""
           ||
           properties.isModified()
           ||
           mList.length()>0;
}

WordXMLText* WordXMLRun::addText(QString aText)
{
    WordXMLText *aNewComponent=new WordXMLText(this);
    aNewComponent->text=aText;
    mList.append(aNewComponent);
    return aNewComponent;
}

WordXMLSeparator* WordXMLRun::addSeparator()
{
    WordXMLSeparator *aNewComponent=new WordXMLSeparator(this);
    mList.append(aNewComponent);
    return aNewComponent;
}

WordXMLContinuationSeparator* WordXMLRun::addContinuationSeparator()
{
    WordXMLContinuationSeparator *aNewComponent=new WordXMLContinuationSeparator(this);
    mList.append(aNewComponent);
    return aNewComponent;
}

WordXMLBreak* WordXMLRun::addBreak()
{
    WordXMLBreak *aNewComponent=new WordXMLBreak(this);
    mList.append(aNewComponent);
    return aNewComponent;
}

WordXMLTabChar* WordXMLRun::addTabChar()
{
    WordXMLTabChar *aNewComponent=new WordXMLTabChar(this);
    mList.append(aNewComponent);
    return aNewComponent;
}

WordXMLPageNumber* WordXMLRun::addPageNumber()
{
    WordXMLPageNumber *aNewComponent=new WordXMLPageNumber(this);
    mList.append(aNewComponent);
    return aNewComponent;
}

WordXMLBase* WordXMLRun::get(int index)
{
    return mList.at(index);
}

void WordXMLRun::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLRun::remove(WordXMLBase* aXMLComponent)
{
    mList.removeOne(aXMLComponent);
}

int WordXMLRun::count()
{
    return mList.length();
}

void WordXMLRun::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}

