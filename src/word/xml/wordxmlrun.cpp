#include "wordxmlrun.h"

WordXMLRun::WordXMLRun(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtRun;

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

void WordXMLRun::setFont(QTextCharFormat aFormat)
{
    properties.setFont(aFormat);
}

void WordXMLRun::setFont(QFont aFont)
{
    properties.setFont(aFont);
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

WordXMLFieldChar* WordXMLRun::addFieldChar(EFieldCharType aType)
{
    WordXMLFieldChar *aNewComponent=new WordXMLFieldChar(this);
    aNewComponent->type=aType;
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

WordXMLRun& WordXMLRun::operator=(const WordXMLRun &another)
{
    rsidR=another.rsidR;
    properties=another.properties;

    clear();

    for (int i=0; i<another.mList.length(); i++)
    {
        WordXMLBase* aNewObject;

        switch (another.mList.at(i)->getComponentType())
        {
            case wxtText:
            {
                aNewObject=new WordXMLText(this);
                *((WordXMLText*)aNewObject)=*((WordXMLText*)(another.mList.at(i)));
            }
            break;
            case wxtSeparator:
            {
                aNewObject=new WordXMLSeparator(this);
                *((WordXMLSeparator*)aNewObject)=*((WordXMLSeparator*)(another.mList.at(i)));
            }
            break;
            case wxtContinuationSeparator:
            {
                aNewObject=new WordXMLContinuationSeparator(this);
                *((WordXMLContinuationSeparator*)aNewObject)=*((WordXMLContinuationSeparator*)(another.mList.at(i)));
            }
            break;
            case wxtBreak:
            {
                aNewObject=new WordXMLBreak(this);
                *((WordXMLBreak*)aNewObject)=*((WordXMLBreak*)(another.mList.at(i)));
            }
            break;
            case wxtTabChar:
            {
                aNewObject=new WordXMLTabChar(this);
                *((WordXMLTabChar*)aNewObject)=*((WordXMLTabChar*)(another.mList.at(i)));
            }
            break;
            case wxtPageNumber:
            {
                aNewObject=new WordXMLPageNumber(this);
                *((WordXMLPageNumber*)aNewObject)=*((WordXMLPageNumber*)(another.mList.at(i)));
            }
            break;
            case wxtFieldChar:
            {
                aNewObject=new WordXMLFieldChar(this);
                *((WordXMLFieldChar*)aNewObject)=*((WordXMLFieldChar*)(another.mList.at(i)));
            }
            break;
            default:
            {
                throw "Wrong component type";
            }
        }

        mList.append(aNewObject);
    }

    return *this;
}
