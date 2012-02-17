#include "wordxmlmultipart.h"

#include "wordxmlparagraph.h"
#include "wordxmltable.h"

WordXMLMultiPart::WordXMLMultiPart(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    reset();
}

WordXMLMultiPart::~WordXMLMultiPart()
{
    clear();
}

void WordXMLMultiPart::writeToStream(QTextStream &aStream)
{
    for (int i=0; i<mList.length(); i++)
    {
        mList.at(i)->writeToStream(aStream);
    }
}

void WordXMLMultiPart::reset()
{
    WordXMLBase::reset();

    clear();
}

bool WordXMLMultiPart::isModified()
{
    return mList.length()>0;
}

void WordXMLMultiPart::insertFromText(QTextEdit *aTextEdit)
{

}

WordXMLParagraph* WordXMLMultiPart::addParagraph()
{
    WordXMLParagraph *aNewComponent=new WordXMLParagraph(this);
    mList.append(aNewComponent);
    return aNewComponent;
}

WordXMLTable* WordXMLMultiPart::addTable()
{
    WordXMLTable *aNewComponent=new WordXMLTable(this);
    mList.append(aNewComponent);
    return aNewComponent;
}

WordXMLBase* WordXMLMultiPart::get(int index)
{
    return mList.at(index);
}

void WordXMLMultiPart::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLMultiPart::remove(WordXMLBase* aXMLComponent)
{
    mList.removeOne(aXMLComponent);
}

int WordXMLMultiPart::count()
{
    return mList.length();
}

void WordXMLMultiPart::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}

