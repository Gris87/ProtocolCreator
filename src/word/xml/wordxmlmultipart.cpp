#include "wordxmlmultipart.h"

#include "wordxmlparagraph.h"
#include "wordxmltable.h"

WordXMLMultiPart::WordXMLMultiPart(WordXMLBase* aParent) : WordXMLBase(aParent)
{
    componentType=wxtMultiPart;

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

void WordXMLMultiPart::insertFromText(QTextFrame *aFrame)
{
    // Range types
    // 0 - Current frame
    // 1 - QTextTable
    QList<int> types;
    QList<int> startRange;
    QList<int> endRange;

    QList<QTextFrame *> aChildren=aFrame->childFrames();

    if (aChildren.length()==0)
    {
        types.append(0);
        startRange.append(aFrame->firstPosition());
        endRange.append(aFrame->lastPosition());
    }
    else
    {
        types.append(0);
        startRange.append(aFrame->firstPosition());
        endRange.append(aChildren.at(0)->firstPosition());

        for (int i=0; i<aChildren.length(); i++)
        {
            if (aChildren.at(i)->inherits("QTextTable"))
            {
                types.append(1);
            }
            else
            {
                types.append(-1);
            }

            startRange.append(aChildren.at(i)->firstPosition());
            endRange.append(aChildren.at(i)->lastPosition());

            if (i<aChildren.length()-1)
            {
                types.append(0);
                startRange.append(aChildren.at(i)->lastPosition());
                endRange.append(aChildren.at(i+1)->firstPosition());
            }
        }

        types.append(0);
        startRange.append(aChildren.at(aChildren.length()-1)->lastPosition());
        endRange.append(aFrame->lastPosition());
    }

    if (aFrame->inherits("QTextTable"))
    {
        //QTextTable *aTable=(QTextTable*)aFrame;
    }
    else
    {
        for (int i=0; i<types.length(); i++)
        {
            int aType=types.at(i);

            switch(aType)
            {
                // Current frame (Text)
                case 0:
                {
                    insertByTextCursor(aFrame->firstCursorPosition(), startRange.at(i), endRange.at(i));
                }
                break;
                case 1:
                {

                }
                break;
                default:
                {
                    throw "Unknown frame type";
                }
                break;
            }
        }
    }
}

void WordXMLMultiPart::insertByTextCursor(QTextCursor cursor, const int start, const int end)
{
    WordXMLParagraph *paragraph=addParagraph();
    WordXMLRun *run=paragraph->addRun();

    QTextCharFormat aTextFormat;
    QString aTextPart="";

    for (int i=start; i<end; i++)
    {
        cursor.setPosition(i);
        cursor.setPosition(i+1, QTextCursor::KeepAnchor);

        QString aSelectedText=cursor.selectedText();

        if (aTextPart=="")
        {
            aTextFormat=cursor.charFormat();
            aTextPart=aSelectedText;

            if (aTextPart==QChar(8233) || aTextPart=="\n")
            {
                run->setFont(aTextFormat);
                aTextPart="";

                paragraph=addParagraph();
                run=paragraph->addRun();
            }
        }
        else
        {
            if (aSelectedText==QChar(8233) || aSelectedText=="\n")
            {
                run->setFont(aTextFormat);
                run->addText(aTextPart);

                paragraph=addParagraph();
                run=paragraph->addRun();

                aTextPart="";
            }
            else
            {
                QTextCharFormat aNextFormat=cursor.charFormat();

                if (aNextFormat==aTextFormat)
                {
                    aTextPart.append(aSelectedText);
                }
                else
                {
                    run->setFont(aTextFormat);
                    run->addText(aTextPart);

                    run=paragraph->addRun();

                    aTextFormat=aNextFormat;
                    aTextPart=aSelectedText;
                }
            }
        }
    }

    if (aTextPart!="")
    {
        run->setFont(aTextFormat);
        run->addText(aTextPart);
    }
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

WordXMLMultiPart& WordXMLMultiPart::operator=(const WordXMLMultiPart &another)
{
    clear();

    for (int i=0; i<another.mList.length(); i++)
    {
        WordXMLBase* aNewObject;

        switch (another.mList.at(i)->getComponentType())
        {
            case wxtParagraph:
            {
                aNewObject=new WordXMLParagraph(this);
                *((WordXMLParagraph*)aNewObject)=*((WordXMLParagraph*)(another.mList.at(i)));
            }
            break;
            case wxtTable:
            {
                aNewObject=new WordXMLTable(this);
                *((WordXMLTable*)aNewObject)=*((WordXMLTable*)(another.mList.at(i)));
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
