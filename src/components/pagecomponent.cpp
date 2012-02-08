#include "src/other/global.h"

PageComponent::PageComponent(QWidget *parent) :
    QWidget(parent)
{
    superParent=parent;

    resetCalculation();
}

QString PageComponent::name()
{
    return "";
}

QString PageComponent::variableName()
{
    return "";
}

void PageComponent::saveToStream(QDataStream &aStream)
{
    // Nothing
}

void PageComponent::loadFromStream(QDataStream &aStream)
{
    // Nothing
}

void PageComponent::updateAdmin()
{
    // Nothing
}

void PageComponent::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    // Nothing
}

void PageComponent::createConnections(const QObject *receiver, const char *memberUp, const char *memberDown, const char *memberCopy, const char *memberDelete)
{
    connect(this, SIGNAL(upPressed(PageComponent*)),     receiver, memberUp);
    connect(this, SIGNAL(downPressed(PageComponent*)),   receiver, memberDown);
    connect(this, SIGNAL(copyPressed(PageComponent*)),   receiver, memberCopy);
    connect(this, SIGNAL(deletePressed(PageComponent*)), receiver, memberDelete);
}

bool PageComponent::isEditable()
{
    return true;
}

void PageComponent::checkForErrors(QStringList &aErrorList)
{
    checkVarName(variableName(), aErrorList);
}

void PageComponent::resetCalculation()
{
    isWasCalculated=false;
    isInCalculation=false;
    calculationError="";
    calculationResult=0;
}

QVariant PageComponent::calculate()
{
    if (isInCalculation)
    {
        calculationError="Зациклилось вычисление";
        throw "This component already in calculation process";
    }

    isInCalculation=true;

    return calculationResult;
}

void PageComponent::getWidgetList(QList<QWidget*> &aList)
{
    // Nothing
}

void PageComponent::setWidgetCursor(QWidget* aWidget, bool isForward)
{
    if (aWidget->inherits("QLineEdit"))
    {
        if (isForward)
        {
            ((QLineEdit*)aWidget)->setCursorPosition(0);
        }
        else
        {
            ((QLineEdit*)aWidget)->setCursorPosition(((QLineEdit*)aWidget)->text().length());
        }
    }
}

bool PageComponent::find(bool isForward)
{
    QList<QWidget*> widgets;
    getWidgetList(widgets);

    if (widgets.length()==0)
    {
        return false;
    }

    int selectIndex=-1;

    QWidget *aLastWidget=focusWidget();

    for (int i=0; i<widgets.length(); i++)
    {
        if (widgets.at(i)==aLastWidget)
        {
            selectIndex=i;
            break;
        }
    }

    if (selectIndex<0)
    {
        if (isForward)
        {
            selectIndex=0;
        }
        else
        {
            selectIndex=widgets.length()-1;
        }

        setWidgetCursor(widgets.at(selectIndex), isForward);
    }

    do
    {
        QWidget *aWidget=widgets.at(selectIndex);

        if (aWidget->inherits("QLineEdit"))
        {
            QString aText=((QLineEdit*)aWidget)->text();
            QString aSelection=((QLineEdit*)aWidget)->selectedText();
            int aStart;

            if (aSelection.length()>0)
            {
                aStart=((QLineEdit*)aWidget)->selectionStart();

                if (aSelection.compare(lastSearch, Qt::CaseInsensitive)==0)
                {
                    if (isReplace)
                    {
                        ((QLineEdit*)aWidget)->insert(lastReplace);
                        aText=((QLineEdit*)aWidget)->text();

                        if (isForward)
                        {
                            aStart+=lastReplace.length();
                        }
                        else
                        {
                            aStart--;
                        }
                    }
                    else
                    {
                        if (isForward)
                        {
                            aStart+=aSelection.length();
                        }
                        else
                        {
                            aStart--;
                        }
                    }
                }
                else
                {
                    if (!isForward)
                    {
                        aStart+=aSelection.length()-1;
                    }
                }
            }
            else
            {
                aStart=((QLineEdit*)aWidget)->cursorPosition();
            }

            int index;

            if (aStart<0)
            {
                index=-1;
            }
            else
            {
                if (isForward)
                {
                    index=aText.indexOf(lastSearch, aStart, Qt::CaseInsensitive);
                }
                else
                {
                    if (aStart==aText.length())
                    {
                        aStart=aText.length()-1;
                    }

                    index=aText.lastIndexOf(lastSearch, aStart, Qt::CaseInsensitive);
                }
            }

            if (index>=0)
            {
                ((QLineEdit*)aWidget)->setSelection(index, lastSearch.length());
                aWidget->setFocus();
                return true;
            }
            else
            {
                ((QLineEdit*)aWidget)->setCursorPosition(0);
            }
        }

        if (isForward)
        {
            selectIndex++;
        }
        else
        {
            selectIndex--;
        }

        if (selectIndex<0 || selectIndex>=widgets.length())
        {
            break;
        }

        setWidgetCursor(widgets.at(selectIndex), isForward);
    } while(true);

    return false;
}
