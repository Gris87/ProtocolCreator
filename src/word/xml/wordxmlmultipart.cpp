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
        QTextTable *aTableFrame=(QTextTable*)aFrame;
        WordXMLTable *aTable=addTable();
        QVector<QTextLength> aWidthList=aTableFrame->format().columnWidthConstraints();

        aTable->properties.borders.setBorders(tbSingle);

        for (int i=0; i<aTableFrame->rows(); i++)
        {
            WordXMLTableRow *aRow=aTable->addRow();

            for (int j=0; j<aTableFrame->columns(); j++)
            {
                WordXMLTableCell *aCell=aRow->addCell();

                aCell->properties.width=aWidthList.at(j).rawValue()*15.044025;
                aCell->properties.columnSpan=aTableFrame->cellAt(i, j).columnSpan();

                if (aTableFrame->cellAt(i, j).format().background().style()!=Qt::NoBrush)
                {
                    aCell->properties.shading.pattern="pct-25";

                    aCell->properties.shading.setColor(aTableFrame->cellAt(i, j).format().background().color());
                    aCell->properties.shading.backgroundColor=aCell->properties.shading.color;
                    aCell->properties.shading.fillColor=aCell->properties.shading.color;
                }

                for (int k=1; k<aCell->properties.columnSpan; k++)
                {
                    aCell->properties.width+=aWidthList.at(j+k).rawValue()*15.044025;
                }

                bool writeCell=true;

                if (aTableFrame->cellAt(i, j).rowSpan()>1)
                {
                    if (aTableFrame->cellAt(i, j).row()==i)
                    {
                        aCell->properties.vMergeType=mtRestart;
                    }
                    else
                    {
                        aCell->properties.vMergeType=mtContinue;
                        writeCell=false;
                    }
                }

                if (writeCell)
                {
                    int start=aTableFrame->cellAt(i, j).firstPosition();
                    int end=aTableFrame->cellAt(i, j).lastPosition();

                    if (types.length()>1)
                    {
                        int insideIndex=-1;

                        for (int k=0; k<types.length(); k++)
                        {
                            int startBlock=startRange.at(k);
                            int endBlock=endRange.at(k);

                            if (startBlock>=start && endBlock<=end)
                            {
                                insideIndex=k;
                                break;
                            }
                        }

                        if (insideIndex>=0)
                        {
                            insideIndex++;

                            do
                            {
                                int startBlock=startRange.at(insideIndex)-1;

                                aCell->insertByTextCursor(aTableFrame->cellAt(i, j).firstCursorPosition(), start, startBlock);
                                aCell->insertFromText(aChildren.at(insideIndex>>1));

                                start=endRange.at(insideIndex)+2;

                                insideIndex+=2;
                            } while(insideIndex<types.length() && endRange.at(insideIndex)<=end);
                        }
                    }

                    aCell->insertByTextCursor(aTableFrame->cellAt(i, j).firstCursorPosition(), start, end);
                }

                if (aCell->properties.columnSpan>1)
                {
                    j+=aCell->properties.columnSpan-1;
                }
            }
        }
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
                    int start=startRange.at(i);
                    int end=endRange.at(i);

                    if (i>0 && types.at(i-1)==1)
                    {
                        start+=2;
                    }

                    if (i<types.length()-1 && types.at(i+1)==1)
                    {
                        end--;
                    }

                    insertByTextCursor(aFrame->firstCursorPosition(), start, end);
                }
                break;
                // Table
                case 1:
                {
                    insertFromText(aChildren.at(i>>1));
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
    if (start>=end)
    {
        return;
    }

    WordXMLParagraph *paragraph=addParagraph();
    WordXMLRun *run=paragraph->addRun();

    cursor.setPosition(start);
    paragraph->setFormat(cursor.blockFormat());

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

                paragraph->setFormat(cursor.blockFormat());
            }
        }
        else
        {
            if (aSelectedText==QChar(8233) || aSelectedText=="\n")
            {
                putTextWithFormat(paragraph, run, aTextPart, aTextFormat);
                aTextPart="";

                paragraph=addParagraph();
                run=paragraph->addRun();

                paragraph->setFormat(cursor.blockFormat());
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
                    putTextWithFormat(paragraph, run, aTextPart, aTextFormat);

                    run=paragraph->addRun();

                    aTextFormat=aNextFormat;
                    aTextPart=aSelectedText;
                }
            }
        }
    }

    if (aTextPart!="")
    {
        putTextWithFormat(paragraph, run, aTextPart, aTextFormat);
    }
}

void WordXMLMultiPart::putTextWithFormat(WordXMLParagraph *paragraph, WordXMLRun *run, QString aText, QTextCharFormat aFormat)
{
    do
    {
        int index=aText.indexOf("#");

        if (index<0)
        {
            break;
        }

        if (index<aText.length()-1 && aText.at(index+1)=='#')
        {
            run->setFont(aFormat);
            run->addText(aText.left(index+1));

            aText.remove(0, index+2);

            if (aText!="")
            {
                run=paragraph->addRun();
            }

            continue;
        }

        int index2=aText.indexOf("#", index+1);

        if (index2<0)
        {
            break;
        }

        run->setFont(aFormat);
        run->addText(aText.left(index));

        QString aAutoText=aText.mid(index+1, index2-index-1);

        aText.remove(0, index2+1);

        run=paragraph->addRun();

        run->setFont(aFormat);
        run->addFieldChar(fctBegin);

        run=paragraph->addRun();

        run->setFont(aFormat);
        run->addText(aAutoText);

        run=paragraph->addRun();

        run->setFont(aFormat);
        run->addFieldChar(fctEnd);

        if (aText!="")
        {
            run=paragraph->addRun();
        }

    } while(true);

    if (aText!="")
    {
        run->setFont(aFormat);
        run->addText(aText);
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
