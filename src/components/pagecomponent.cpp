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
    else
    if (
        aWidget->inherits("QSpinBox")
        ||
        aWidget->inherits("QDoubleSpinBox")
        ||
        aWidget->inherits("QDateEdit")
        ||
        aWidget->inherits("QTimeEdit")
       )
    {
        ((MySpinBox*)aWidget)->lineEdit()->setCursorPosition(0);
    }
    else
    if (aWidget->inherits("QPlainTextEdit"))
    {
        if (isForward)
        {
            ((QPlainTextEdit*)aWidget)->moveCursor(QTextCursor::Start);
        }
        else
        {
            ((QPlainTextEdit*)aWidget)->moveCursor(QTextCursor::End);
        }
    }
    else
    if (aWidget->inherits("QTextEdit"))
    {
        if (isForward)
        {
            ((QTextEdit*)aWidget)->moveCursor(QTextCursor::Start);
        }
        else
        {
            ((QTextEdit*)aWidget)->moveCursor(QTextCursor::End);
        }
    }
    else
    if (aWidget->inherits("QComboBox"))
    {
        if (isForward)
        {
            ((QComboBox*)aWidget)->lineEdit()->setCursorPosition(0);
        }
        else
        {
            ((QComboBox*)aWidget)->lineEdit()->setCursorPosition(((QComboBox*)aWidget)->currentText().length());
        }
    }
    else
    if (aWidget->inherits("DataTable"))
    {
        VariableExtendedListFrame *aTable=(VariableExtendedListFrame*)((DataTable*)aWidget)->mTable;

        if (
            aTable->ui->dataTableWidget->rowCount()>0
            &&
            aTable->ui->dataTableWidget->columnCount()>0
           )
        {
            if (isForward)
            {
                aTable->ui->dataTableWidget->setCurrentCell(0, 0, QItemSelectionModel::ClearAndSelect);
            }
            else
            {
                aTable->ui->dataTableWidget->setCurrentCell(aTable->ui->dataTableWidget->rowCount()-1, aTable->ui->dataTableWidget->columnCount()-1, QItemSelectionModel::ClearAndSelect);
            }
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
                aWidget->activateWindow();
                aWidget->setFocus();
                return true;
            }
            else
            {
                ((QLineEdit*)aWidget)->setCursorPosition(0);
            }
        }
        else
        if (aWidget->inherits("QSpinBox"))
        {
            QString aText=((MySpinBox*)aWidget)->lineEdit()->text();

            if (aText==lastSearch)
            {
                if (((MySpinBox*)aWidget)->lineEdit()->selectedText()==lastSearch)
                {
                    if (isReplace)
                    {
                        bool ok;
                        int aValue=lastReplace.toInt(&ok);

                        if (ok)
                        {
                            ((QSpinBox*)aWidget)->setValue(aValue);
                        }
                    }

                    ((MySpinBox*)aWidget)->lineEdit()->setCursorPosition(0);
                }
                else
                {
                    ((MySpinBox*)aWidget)->selectAll();
                    aWidget->activateWindow();
                    aWidget->setFocus();
                    return true;
                }
            }
            else
            {
                ((MySpinBox*)aWidget)->lineEdit()->setCursorPosition(0);
            }
        }
        else
        if (aWidget->inherits("QDoubleSpinBox"))
        {
            bool ok;
            double searchValue=lastSearch.toDouble(&ok);

            if (ok)
            {
                if (((QDoubleSpinBox*)aWidget)->value()==searchValue)
                {
                    if (((MySpinBox*)aWidget)->lineEdit()->selectedText()!="")
                    {
                        if (isReplace)
                        {
                            double aValue=lastReplace.toDouble(&ok);

                            if (ok)
                            {
                                ((QDoubleSpinBox*)aWidget)->setValue(aValue);
                            }
                        }

                        ((MySpinBox*)aWidget)->lineEdit()->setCursorPosition(0);
                    }
                    else
                    {
                        ((MySpinBox*)aWidget)->selectAll();
                        aWidget->activateWindow();
                        aWidget->setFocus();
                        return true;
                    }
                }
            }
        }
        else
        if (aWidget->inherits("QDateEdit"))
        {
            QString aText=((MySpinBox*)aWidget)->lineEdit()->text();

            if (aText==lastSearch)
            {
                if (((MySpinBox*)aWidget)->lineEdit()->selectedText()==lastSearch)
                {
                    if (isReplace)
                    {
                        QDate aValue=QDate::fromString(lastReplace, "dd.MM.yyyy");

                        if (aValue.isValid())
                        {
                            ((QDateEdit*)aWidget)->setDate(aValue);
                        }
                    }

                    ((MySpinBox*)aWidget)->lineEdit()->setCursorPosition(0);
                }
                else
                {
                    ((MySpinBox*)aWidget)->selectAll();
                    aWidget->activateWindow();
                    aWidget->setFocus();
                    return true;
                }
            }
            else
            {
                ((MySpinBox*)aWidget)->lineEdit()->setCursorPosition(0);
            }
        }
        else
        if (aWidget->inherits("QTimeEdit"))
        {
            QString aText=((MySpinBox*)aWidget)->lineEdit()->text();

            if (aText==lastSearch)
            {
                if (((MySpinBox*)aWidget)->lineEdit()->selectedText()==lastSearch)
                {
                    if (isReplace)
                    {
                        QTime aValue=QTime::fromString(lastReplace, "hh:mm:ss");

                        if (aValue.isValid())
                        {
                            ((QTimeEdit*)aWidget)->setTime(aValue);
                        }
                    }

                    ((MySpinBox*)aWidget)->lineEdit()->setCursorPosition(0);
                }
                else
                {
                    ((MySpinBox*)aWidget)->selectAll();
                    aWidget->activateWindow();
                    aWidget->setFocus();
                    return true;
                }
            }
            else
            {
                ((MySpinBox*)aWidget)->lineEdit()->setCursorPosition(0);
            }
        }
        else
        if (aWidget->inherits("QPlainTextEdit"))
        {
            QString aText=((QPlainTextEdit*)aWidget)->toPlainText();
            QString aSelection=((QPlainTextEdit*)aWidget)->textCursor().selectedText();
            int aStart;

            if (aSelection.length()>0)
            {
                aStart=((QPlainTextEdit*)aWidget)->textCursor().selectionStart();

                if (aSelection.compare(lastSearch, Qt::CaseInsensitive)==0)
                {
                    if (isReplace)
                    {
                        ((QPlainTextEdit*)aWidget)->textCursor().insertText(lastReplace);
                        aText=((QPlainTextEdit*)aWidget)->toPlainText();

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
                aStart=((QPlainTextEdit*)aWidget)->textCursor().position();
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
                QTextCursor aCursor=((QPlainTextEdit*)aWidget)->textCursor();

                aCursor.setPosition(index);
                aCursor.setPosition(index+lastSearch.length(), QTextCursor::KeepAnchor);

                ((QPlainTextEdit*)aWidget)->setTextCursor(aCursor);
                aWidget->activateWindow();
                aWidget->setFocus();
                return true;
            }
            else
            {
                ((QPlainTextEdit*)aWidget)->moveCursor(QTextCursor::Start);
            }
        }
        else
        if (aWidget->inherits("QTextEdit"))
        {
            QString aText=((QTextEdit*)aWidget)->toPlainText();
            QString aSelection=((QTextEdit*)aWidget)->textCursor().selectedText();
            int aStart;

            if (aSelection.length()>0)
            {
                aStart=((QTextEdit*)aWidget)->textCursor().selectionStart();

                if (aSelection.compare(lastSearch, Qt::CaseInsensitive)==0)
                {
                    if (isReplace)
                    {
                        QTextCursor aCursor=((QTextEdit*)aWidget)->textCursor();

                        aCursor.setPosition(aCursor.selectionStart()+1);

                        ((QTextEdit*)aWidget)->textCursor().insertText(lastReplace, aCursor.charFormat());
                        aText=((QTextEdit*)aWidget)->toPlainText();

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
                aStart=((QTextEdit*)aWidget)->textCursor().position();
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
                QTextCursor aCursor=((QTextEdit*)aWidget)->textCursor();

                aCursor.setPosition(index);
                aCursor.setPosition(index+lastSearch.length(), QTextCursor::KeepAnchor);

                ((QTextEdit*)aWidget)->setTextCursor(aCursor);
                aWidget->activateWindow();
                aWidget->setFocus();
                return true;
            }
            else
            {
                ((QTextEdit*)aWidget)->moveCursor(QTextCursor::Start);
            }
        }
        else
        if (aWidget->inherits("QComboBox"))
        {
            QString aText=((QComboBox*)aWidget)->currentText();
            QString aSelection=((QComboBox*)aWidget)->lineEdit()->selectedText();
            int aStart;

            if (aSelection.length()>0)
            {
                aStart=((QComboBox*)aWidget)->lineEdit()->selectionStart();

                if (aSelection.compare(lastSearch, Qt::CaseInsensitive)==0)
                {
                    if (isReplace)
                    {
                        ((QComboBox*)aWidget)->lineEdit()->insert(lastReplace);
                        aText=((QComboBox*)aWidget)->currentText();

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
                aStart=((QComboBox*)aWidget)->lineEdit()->cursorPosition();
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
                ((QComboBox*)aWidget)->lineEdit()->setSelection(index, lastSearch.length());
                aWidget->activateWindow();
                aWidget->setFocus();
                return true;
            }
            else
            {
                ((QComboBox*)aWidget)->lineEdit()->setCursorPosition(0);
            }
        }
        else
        if (aWidget->inherits("DataTable"))
        {
            VariableExtendedListFrame *aTable=(VariableExtendedListFrame*)((DataTable*)aWidget)->mTable;
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

QLineEdit* MySpinBox::lineEdit()
{
    return QAbstractSpinBox::lineEdit();
}
