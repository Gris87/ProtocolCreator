#include "wordxmlsections.h"

WordXMLSections::WordXMLSections(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtSections;

    reset();
}

WordXMLSections::~WordXMLSections()
{
    clear();
}

void WordXMLSections::writeToStream(QTextStream &aStream)
{
    if (isModified())
    {
        aStream<<space<<"<w:body>\r\n";

        for (int i=0; i<mList.length(); i++)
        {
            if (i>0)
            {
                aStream<<"\r\n";
            }

            mList.at(i)->writeToStream(aStream);
        }

        aStream<<space<<"</w:body>\r\n";
    }
}

void WordXMLSections::reset()
{
    WordXMLBase::reset();

    clear();
}

bool WordXMLSections::isModified()
{
    return mList.length()>0;
}

WordXMLSection* WordXMLSections::add()
{
    WordXMLSection* aNewSection=new WordXMLSection(this);
    mList.append(aNewSection);
    return aNewSection;
}

WordXMLSection* WordXMLSections::copy(int index)
{
    return copy(mList.at(index));
}

WordXMLSection* WordXMLSections::copy(WordXMLSection* aSection)
{
    WordXMLSection* aNewSection=new WordXMLSection(this);

    *aNewSection=*aSection;

    mList.append(aNewSection);

    return aNewSection;
}

WordXMLSection* WordXMLSections::get(int index)
{
    return mList.at(index);
}

void WordXMLSections::remove(int index)
{
    mList.removeAt(index);
}

void WordXMLSections::remove(WordXMLSection* aSection)
{
    mList.removeOne(aSection);
}

int WordXMLSections::count()
{
    return mList.length();
}

void WordXMLSections::clear()
{
    for (int i=0; i<mList.length(); i++)
    {
        delete mList[i];
    }

    mList.clear();
}

WordXMLSections& WordXMLSections::operator=(const WordXMLSections &another)
{
    clear();

    for (int i=0; i<another.mList.length(); i++)
    {
        WordXMLSection* aNewObject=new WordXMLSection(this);
        *aNewObject=*(another.mList.at(i));
        mList.append(aNewObject);
    }

    return *this;
}
