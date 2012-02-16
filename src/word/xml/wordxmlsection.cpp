#include "wordxmlsection.h"

WordXMLSection::WordXMLSection(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    properties.parent=this;

    reset();
}

WordXMLSection::~WordXMLSection()
{
    clear();
}

void WordXMLSection::writeToStream(QTextStream &aStream)
{
    aStream<<space<<"<w:sect>\r\n";

    properties.writeToStream(aStream);

    for (int i=0; i<mList.length(); i++)
    {
        mList.at(i)->writeToStream(aStream);
    }

    aStream<<space<<"</w:sect>\r\n";
}

void WordXMLSection::reset()
{
    WordXMLBase::reset();

    properties.reset();

    clear();
}

bool WordXMLSection::isModified()
{
    return properties.isModified()
           ||
           mList.length()>0;
}

WordXMLParagraph* WordXMLSection::addParagraph()
{
    WordXMLParagraph *aNewComponent=new WordXMLParagraph(this);
    mList.append(aNewComponent);
    return aNewComponent;
}

WordXMLBase* WordXMLSection::get(int index)
{
    return mList.at(index);
}

void WordXMLSection::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLSection::remove(WordXMLBase* aXMLComponent)
{
    mList.removeOne(aXMLComponent);
}

int WordXMLSection::count()
{
    return mList.length();
}

void WordXMLSection::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}

