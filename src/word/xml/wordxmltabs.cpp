#include "wordxmltabs.h"

WordXMLTabs::WordXMLTabs(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtTabs;

    reset();
}

WordXMLTabs::~WordXMLTabs()
{
    clear();
}

void WordXMLTabs::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:tabs>\r\n";

        for (int i=0; i<mList.length(); i++)
        {
            mList.at(i)->writeToStream(aStream);
        }

        aStream<<space<<"</w:tabs>\r\n";
    }
}

void WordXMLTabs::reset()
{
    WordXMLBase::reset();

    clear();
}

bool WordXMLTabs::isModified()
{
    return mList.length()>0;
}

WordXMLTab* WordXMLTabs::add(ETabLocation aLocation, int aPosition)
{
    WordXMLTab* aNewTab=new WordXMLTab(this);

    aNewTab->location=aLocation;
    aNewTab->position=aPosition;

    mList.append(aNewTab);

    return aNewTab;
}

WordXMLTab* WordXMLTabs::copy(int index, int aPosition)
{
    return copy(mList.at(index), aPosition);
}

WordXMLTab* WordXMLTabs::copy(WordXMLTab* aTab, int aPosition)
{
    WordXMLTab* aNewTab=new WordXMLTab(this);

    *aNewTab=*aTab;
    aNewTab->position=aPosition;

    mList.append(aNewTab);

    return aNewTab;
}

WordXMLTab* WordXMLTabs::get(int index)
{
    return mList.at(index);
}

void WordXMLTabs::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLTabs::remove(WordXMLTab* aTab)
{
    mList.removeOne(aTab);
}

int WordXMLTabs::count()
{
    return mList.length();
}

void WordXMLTabs::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}

WordXMLTabs& WordXMLTabs::operator=(const WordXMLTabs &another)
{
    clear();

    for (int i=0; i<another.mList.length(); i++)
    {
        WordXMLTab* aNewObject=new WordXMLTab(this);
        *aNewObject=*(another.mList.at(i));
        mList.append(aNewObject);
    }

    return *this;
}
