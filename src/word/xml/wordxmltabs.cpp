#include "wordxmltabs.h"

WordXMLTabs::WordXMLTabs() : WordXMLBase()
{
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
        aStream<<"    <w:tabs>\r\n";

        for (int i=0; i<tabsList.length(); i++)
        {
            tabsList.at(i)->writeToStream(aStream);
        }

        aStream<<"    </w:tabs>\r\n";
    }
}

void WordXMLTabs::reset()
{
    clear();
}

bool WordXMLTabs::isModified()
{
    return tabsList.length()>0;
}

WordXMLTab* WordXMLTabs::add(ETabLocation aLocation, int aPosition)
{
    WordXMLTab* aNewTab=new WordXMLTab();

    aNewTab->location=aLocation;
    aNewTab->position=aPosition;

    tabsList.append(aNewTab);

    return aNewTab;
}

WordXMLTab* WordXMLTabs::copy(int index, int aPosition)
{
    return copy(tabsList.at(index), aPosition);
}

WordXMLTab* WordXMLTabs::copy(WordXMLTab* aTab, int aPosition)
{
    WordXMLTab* aNewTab=new WordXMLTab();

    *aNewTab=*aTab;
    aNewTab->position=aPosition;

    tabsList.append(aNewTab);

    return aNewTab;
}

WordXMLTab* WordXMLTabs::get(int index)
{
    return tabsList.at(index);
}

void WordXMLTabs::remove(int index)
{
    tabsList.removeAt(index);
}

void WordXMLTabs::remove(WordXMLTab* aTab)
{
    tabsList.removeOne(aTab);
}

int WordXMLTabs::count()
{
    return tabsList.length();
}

void WordXMLTabs::clear()
{
    for (int i=0; i<tabsList.length(); i++)
    {
        delete tabsList[i];
    }

    tabsList.clear();
}