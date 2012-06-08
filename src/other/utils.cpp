#include "src/other/global.h"

void myMessageOutput(QtMsgType type, const char *msg)
{
    FILE *file;
    file = fopen("ProtocolCreator.log", "a");

    QString aRow=QDateTime::currentDateTime().toString("MM-dd hh:mm:ss.zzz");

    aRow.append(": ");

    switch (type)
    {
        case QtDebugMsg:
            aRow.append("DEBUG");
        break;
        case QtWarningMsg:
            aRow.append("INFO");
        break;
        case QtCriticalMsg:
            aRow.append("WARN");
        break;
        case QtFatalMsg:
            aRow.append("ERROR");
        break;
    }

    aRow.append("/ProtocolCreator(9999): ");
    aRow.append(QString::fromLocal8Bit(msg));

    fprintf(file, "%s\n", aRow.toUtf8().data());

    fclose(file);
}

void moveFile(const QString aSourceFileName, const QString aDestFileName)
{
    copyFile(aSourceFileName, aDestFileName);

    QFile::remove(aSourceFileName);
}

void copyFile(const QString aSourceFileName, const QString aDestFileName)
{
    QFile aFile(aSourceFileName);

    if (aFile.exists())
    {
        QFile aFile2(aDestFileName);

        aFile.open(QIODevice::ReadOnly);
        aFile2.open(QIODevice::WriteOnly);

        while (!aFile.atEnd())
        {
            aFile2.write(aFile.read(4096));
        }

        aFile.close();
        aFile2.close();
    }
}

void setGeometryInDesktop(QWidget* aWidget, int aX, int aY, int aWidthSize, int aHeightSize)
{
    QDesktopWidget *desktop = QApplication::desktop();
    int aWidth = desktop->width();
    int aHeight = desktop->height();

    if (aX<0)
    {
        aX=0;
    }

    if (aY<0)
    {
        aY=0;
    }

    if (aX+aWidthSize>aWidth)
    {
        aX=aWidth-aWidthSize;
    }

    if (aY+aHeightSize>aHeight)
    {
        aY=aHeight-aHeightSize;
    }

    aWidget->setGeometry(aX, aY, aWidthSize, aHeightSize);
}

void findFocus(int &pageIndex, int &varIndex, int &compIndex)
{
    pageIndex=-1;
    varIndex=-1;
    compIndex=-1;

    if (fullDialog)
    {
        pageIndex=-2;
        return;
    }

    if (globalDialog->isVisible())
    {
        QWidget* aFocusWidget=globalDialog->focusWidget();

        for (int i=0; i<globalDialog->variables.length(); i++)
        {
            if (globalDialog->variables.at(i)->isAncestorOf(aFocusWidget))
            {
                varIndex=i;
                return;
            }
        }
    }
    else
    {
        pageIndex=mainWindow->ui->pagesTabWidget->currentIndex();
        PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(pageIndex);

        QWidget* aFocusWidget=mainWindow->focusWidget();

        for (int i=0; i<aPage->variables.length(); i++)
        {
            if (aPage->variables.at(i)->isAncestorOf(aFocusWidget))
            {
                varIndex=i;
                return;
            }
        }

        for (int i=0; i<aPage->components.length(); i++)
        {
            if (aPage->components.at(i)->isAncestorOf(aFocusWidget))
            {
                compIndex=i;
                return;
            }
        }
    }
}

void copyVariable(PageComponent *aComponent)
{
    PageSelectionDialog dialog(mainWindow);

    dialog.ui->titleLabel->setText("Выберите раздел, в который будет скопирована переменная:");

    if (dialog.exec())
    {
        int aRow=dialog.ui->pagesListWidget->currentRow();

        if (aRow>0 && mainWindow->ui->pagesTabWidget->widget(aRow-1)==mainWindow->contentPage)
        {
            QMessageBox::information(mainWindow, protocolCreatorVersion, "Нельзя вставлять переменную в содержание");
            return;
        }

        QByteArray aBuffer;
        QDataStream aStream(&aBuffer, QIODevice::ReadWrite);

        aComponent->saveToStream(aStream);

        aStream.device()->seek(0);

        QString aMagicWord;
        aStream >> aMagicWord;

        PageComponent *aVariable=0;

        if (aMagicWord=="VarInteger")
        {
            aVariable=new VariableIntegerFrame(aRow==0 ? globalDialog : mainWindow->ui->pagesTabWidget->widget(aRow-1));
        }
        else
        if (aMagicWord=="VarString")
        {
            aVariable=new VariableStringFrame(aRow==0 ? globalDialog : mainWindow->ui->pagesTabWidget->widget(aRow-1));
        }
        else
        if (aMagicWord=="VarBoolean")
        {
            aVariable=new VariableBoolFrame(aRow==0 ? globalDialog : mainWindow->ui->pagesTabWidget->widget(aRow-1));
        }
        else
        if (aMagicWord=="VarDate")
        {
            aVariable=new VariableDateFrame(aRow==0 ? globalDialog : mainWindow->ui->pagesTabWidget->widget(aRow-1));
        }
        else
        if (aMagicWord=="VarTime")
        {
            aVariable=new VariableTimeFrame(aRow==0 ? globalDialog : mainWindow->ui->pagesTabWidget->widget(aRow-1));
        }
        else
        if (aMagicWord=="VarList")
        {
            aVariable=new VariableListFrame(aRow==0 ? globalDialog : mainWindow->ui->pagesTabWidget->widget(aRow-1));
        }
        else
        if (aMagicWord=="VarExtendedList")
        {
            aVariable=new VariableExtendedListFrame(aRow==0 ? globalDialog : mainWindow->ui->pagesTabWidget->widget(aRow-1));

            ((VariableExtendedListFrame*)aVariable)->mIsTable=false;
            ((VariableExtendedListFrame*)aVariable)->ui->useCheckBox->setVisible(false);
            ((VariableExtendedListFrame*)aVariable)->ui->switchButton->setVisible(aRow>0);
        }
        else
        if (aMagicWord=="VarExpression")
        {
            aVariable=new VariableExpressionFrame(aRow==0 ? globalDialog : mainWindow->ui->pagesTabWidget->widget(aRow-1));
        }

        if (aVariable)
        {
            if (aRow==0)
            {
                globalDialog->addVariable(aVariable);
            }
            else
            {
                ((PageFrame*)mainWindow->ui->pagesTabWidget->widget(aRow-1))->addVariable(aVariable);
            }

            aVariable->loadFromStream(aStream);
        }
    }
}

void checkVarName(QString aName, QStringList &aErrorList)
{
    for (int i=0; i<aName.length(); i++)
    {
        if (
            !(aName.at(i)>='a' && aName.at(i)<='z')
            &&
            !(aName.at(i)>='A' && aName.at(i)<='Z')
            &&
            !(aName.at(i)>='а' && aName.at(i)<='я')
            &&
            !(aName.at(i)>='А' && aName.at(i)<='Я')
            &&
            !(aName.at(i)>='0' && aName.at(i)<='9')
            &&
            aName.at(i)!='_'
           )
        {
            aErrorList.append("Error: Недопустимый символ \""+QString(aName.at(i))+"\" в имени");
        }
    }

    if (aName.length()>0 && aName.at(0)>='0' && aName.at(0)<='9')
    {
        aErrorList.append("Error: Имя не может начинаться с цифры");
    }
}

PageComponent* variableByName(QString aVariableName, PageComponent *aComponent)
{
    PageComponent *aVariable=0;

    int dopIndex=aVariableName.indexOf(".");

    if (dopIndex>=0)
    {
        QString aSection=aVariableName.left(dopIndex);
        aVariableName.remove(0, dopIndex+1);

        if (aSection=="Global")
        {
            for (int i=0; i<globalDialog->variables.length(); i++)
            {
                if (globalDialog->variables.at(i)->variableName()==aVariableName)
                {
                    aVariable=globalDialog->variables.at(i);
                    break;
                }
            }
        }
        else
        {
            PageFrame *aPage=0;

            for (int i=0; i<mainWindow->ui->pagesTabWidget->count(); i++)
            {
                if (((PageFrame *)mainWindow->ui->pagesTabWidget->widget(i))->ui->varNameEdit->text()==aSection)
                {
                    aPage=(PageFrame *)mainWindow->ui->pagesTabWidget->widget(i);
                    break;
                }
            }

            if (aPage)
            {
                for (int i=0; i<aPage->variables.length(); i++)
                {
                    if (aPage->variables.at(i)->variableName()==aVariableName)
                    {
                        aVariable=aPage->variables.at(i);
                        break;
                    }
                }

                if (aVariable==0)
                {
                    for (int i=0; i<aPage->components.length(); i++)
                    {
                        if (aPage->components.at(i)->variableName()==aVariableName)
                        {
                            aVariable=aPage->components.at(i);
                            break;
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (aComponent && aComponent->superParent!=globalDialog)
        {
            PageFrame *aPage=(PageFrame*)aComponent->superParent;

            for (int i=0; i<aPage->variables.length(); i++)
            {
                if (aPage->variables.at(i)->variableName()==aVariableName)
                {
                    aVariable=aPage->variables.at(i);
                    break;
                }
            }

            if (aVariable==0)
            {
                for (int i=0; i<aPage->components.length(); i++)
                {
                    if (aPage->components.at(i)->variableName()==aVariableName)
                    {
                        aVariable=aPage->components.at(i);
                        break;
                    }
                }
            }
        }

        if (aVariable==0)
        {
            for (int i=0; i<globalDialog->variables.length(); i++)
            {
                if (globalDialog->variables.at(i)->variableName()==aVariableName)
                {
                    aVariable=globalDialog->variables.at(i);
                    break;
                }
            }
        }
    }

    return aVariable;
}

PageComponent* getVariableOrThrow(QString aVariableName, PageComponent *aComponent)
{
    PageComponent *aVariable=variableByName(aVariableName, aComponent);

    if (aVariable==0)
    {
        int dopIndex=aVariableName.indexOf(".");

        if (dopIndex>=0)
        {
            QString aSection=aVariableName.left(dopIndex);
            aVariableName.remove(0, dopIndex+1);

            if (aSection=="Global")
            {
                aComponent->calculationError="Не найдена глобальная переменная \""+aVariableName+"\"";
            }
            else
            {
                aComponent->calculationError="Не найдена переменная \""+aVariableName+"\" в разделе \""+aSection+"\"";
            }
        }
        else
        {
            aComponent->calculationError="Не найдена переменная \""+aVariableName+"\"";
        }

        throw "Variable not found";
    }

    return aVariable;
}

enum EGOSTDelimiter {gdPoint, gdMinus, gdMultiPoint, gdMultiMinus};

void readGOST(PageComponent *aComponent, QString &aOneLine, QStringList &resList, QString aPrefix, QString aFormat, QString aLocation, QList<EGOSTDelimiter> aDelimiters)
{
    QString aOriginalLine=aOneLine;

    aFormat="Формат написания стандарта: "+aFormat;

    QString aGOST=aPrefix;
    aOneLine.remove(0, aPrefix.length());

    if (aPrefix=="СО")
    {
        if (aOneLine.startsWith(" "))
        {
            aOneLine.remove(0, 1);
        }

        if (aOneLine.startsWith("-"))
        {
            aOneLine.remove(0, 1);
        }

        aGOST.append("-");
    }
    else
    {
        aGOST.append(" ");
    }

    if (aOneLine.startsWith(" "))
    {
        aOneLine.remove(0, 1);
    }

    if (aOneLine=="")
    {
        aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
        throw "Wrong format";
    }

    for (int i=0; i<=aDelimiters.length(); i++)
    {
        QString aPart;

        while (aOneLine.length()>0 && aOneLine.at(0).isDigit())
        {
            aPart.append(aOneLine.at(0));
            aOneLine.remove(0, 1);
        }

        if (
            aPart==""
            &&
            (
             i==0
             ||
             (
              aDelimiters.at(i-1)!=gdMultiPoint
              &&
              aDelimiters.at(i-1)!=gdMultiMinus
             )
            )
           )
        {
            aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
            throw "Wrong format";
        }

        if (aOneLine.startsWith(" "))
        {
            aOneLine.remove(0, 1);
        }

        aGOST.append(aPart);

        if (i<aDelimiters.length())
        {
            if (aDelimiters.at(i)==gdPoint)
            {
                if (!aOneLine.startsWith("."))
                {
                    aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                    throw "Wrong format";
                }

                aGOST.append(".");
                aOneLine.remove(0, 1);

                if (aOneLine.startsWith(" "))
                {
                    aOneLine.remove(0, 1);
                }
            }
            else
            if (aDelimiters.at(i)==gdMinus)
            {
                if (!aOneLine.startsWith("-"))
                {
                    aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                    throw "Wrong format";
                }

                aGOST.append("-");
                aOneLine.remove(0, 1);

                if (aOneLine.startsWith(" "))
                {
                    aOneLine.remove(0, 1);
                }
            }
            else
            {
                while (
                       (
                        aDelimiters.at(i)==gdMultiPoint
                        &&
                        aOneLine.startsWith(".")
                       )
                       ||
                       (
                        aDelimiters.at(i)==gdMultiMinus
                        &&
                        aOneLine.startsWith("-")
                       )
                      )
                {
                    aOneLine.remove(0, 1);

                    if (aOneLine.startsWith(" "))
                    {
                        aOneLine.remove(0, 1);
                    }

                    aPart="";

                    while (aOneLine.length()>0 && aOneLine.at(0).isDigit())
                    {
                        aPart.append(aOneLine.at(0));
                        aOneLine.remove(0, 1);
                    }

                    if (aPart=="")
                    {
                        aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                        throw "Wrong format";
                    }

                    if (aOneLine.startsWith(" "))
                    {
                        aOneLine.remove(0, 1);
                    }

                    if (aDelimiters.at(i)==gdMultiPoint)
                    {
                        aGOST.append(".");
                    }
                    else
                    {
                        aGOST.append("-");
                    }

                    aGOST.append(aPart);
                }
            }
        }
    }

    if (!resList.contains(aGOST))
    {
        resList.append(aGOST);
    }

    while (
           aOneLine!=""
           &&
           !aOneLine.startsWith("ПУЭ")
           &&
           !aOneLine.startsWith("ГОСТ")
           &&
           !aOneLine.startsWith("РД")
           &&
           !aOneLine.startsWith("СО")
           &&
           !aOneLine.startsWith("СП")
           &&
           !aOneLine.startsWith("ТСН")
          )
    {
        aOneLine.remove(0, 1);
    }
}

QVariant calculatePart(QString aExpression, QStringList *aErrorList, PageComponent *aComponent, VariableExtendedListFrame *inFrame, int tableRow)
{
    aExpression=aExpression.trimmed();

    QChar quote='0';
    int bracketCount=0;
    int firstBracket=-1;

    for (int i=0; i<aExpression.length(); i++)
    {
        if (aExpression.at(i)=='\"')
        {
            if (quote=='\"')
            {
                quote='0';
            }
            else
            if (quote=='0')
            {
                quote='\"';
            }
        }
        else
        if (aExpression.at(i)=='\'')
        {
            if (quote=='\'')
            {
                quote='0';
            }
            else
            if (quote=='0')
            {
                quote='\'';
            }
        }
        else
        if (aExpression.at(i)=='\\')
        {
            if (quote!='0')
            {
                i++;
            }
            else
            {
                if (i<aExpression.length()-1)
                {
                    aComponent->calculationError="Специальный символ \\"+QString(aExpression.at(i+1))+" обнаружен вне строки";
                    throw "Special symbol outside of string";
                }
                else
                {
                    aComponent->calculationError="Не достаточно символов";
                    throw "Not enough symbols";
                }
            }
        }
        else
        if (aExpression.at(i).isSpace())
        {
            if (quote=='0')
            {
                aExpression.remove(i, 1);
                i--;
            }
        }
        else
        if (aExpression.at(i)=='(')
        {
            if (quote=='0')
            {
                bracketCount++;

                if (firstBracket<0)
                {
                    firstBracket=i;
                }
            }
        }
        else
        if (aExpression.at(i)==')')
        {
            if (quote=='0')
            {
                bracketCount--;

                if (bracketCount<0)
                {
                    aComponent->calculationError="Не обнаружена открывающаяся скобка \"(\" для закрывающейся \")\"";
                    throw "No open bracket";
                }

                if (bracketCount==0 && i<aExpression.length()-1)
                {
                    aComponent->calculationError="Выражение должно было окончиться закрывающейся скобкой \")\", однако следует текст \""+aExpression.mid(i+1)+"\"";
                    throw "Wrong end";
                }
            }
        }
    }

    if (bracketCount>0)
    {
        aComponent->calculationError="Не обнаружена закрывающаяся скобка \")\" для открывающейся \"(\"";
        throw "No close bracket";
    }

    if (quote!='0')
    {
        aComponent->calculationError="Не найдена закрывающаяся ковычка "+QString(quote);
        throw "Quote not found";
    }

    if (aExpression=="")
    {
        return "";
    }

    if (firstBracket>=0)
    {
        QString aFunction=aExpression.left(firstBracket);

        if (aFunction=="")
        {
            aComponent->calculationError="Перед открывающейся скобкой \"(\" должна быть указана функция";
            throw "Function not found";
        }

        aExpression=aExpression.mid(firstBracket+1);
        aExpression.remove(aExpression.length()-1, 1);

        QStringList aParameters;

        quote='0';
        bracketCount=0;

        for (int i=0; i<aExpression.length(); i++)
        {
            if (aExpression.at(i)=='\"')
            {
                if (quote=='\"')
                {
                    quote='0';
                }
                else
                if (quote=='0')
                {
                    quote='\"';
                }
            }
            else
            if (aExpression.at(i)=='\'')
            {
                if (quote=='\'')
                {
                    quote='0';
                }
                else
                if (quote=='0')
                {
                    quote='\'';
                }
            }
            else
            if (aExpression.at(i)=='\\')
            {
                if (quote!='0')
                {
                    i++;
                }
            }
            else
            if (aExpression.at(i)==';')
            {
                if (quote=='0' && bracketCount==0)
                {
                    aParameters.append(aExpression.left(i));

                    aExpression.remove(0, i+1);
                    i=-1;
                }
            }
            else
            if (aExpression.at(i)=='(')
            {
                if (quote=='0')
                {
                    bracketCount++;
                }
            }
            else
            if (aExpression.at(i)==')')
            {
                if (quote=='0')
                {
                    bracketCount--;
                }
            }
        }

        if (aExpression!="")
        {
            aParameters.append(aExpression);
        }

        bool good=false;

        for (int i=0; i<functionsList.length(); i++)
        {
            if (functionsList.at(i).startsWith(aFunction+"_"))
            {
                bool ok;
                int paramCount=functionsList.at(i).mid(aFunction.length()+1).toInt(&ok);

                if (ok)
                {
                    good=true;

                    if (aParameters.length()!=paramCount)
                    {
                        if (paramCount==0)
                        {
                            aComponent->calculationError="Функция \""+aFunction+"\" не должна содержать параметров";
                        }
                        else
                        if (paramCount==1)
                        {
                            aComponent->calculationError="Функция \""+aFunction+"\" должна содержать 1 параметр";
                        }
                        else
                        if (paramCount<5)
                        {
                            aComponent->calculationError="Функция \""+aFunction+"\" должна содержать "+QString::number(paramCount)+" параметрa";
                        }
                        else
                        {
                            aComponent->calculationError="Функция \""+aFunction+"\" должна содержать "+QString::number(paramCount)+" параметров";
                        }

                        throw "Wrong parameters count";
                    }

                    break;
                }
            }
        }

        if (!good)
        {
            aComponent->calculationError="Неизвестная функция \""+aFunction+"\"";
            throw "Unknown function";
        }

        if (aFunction=="Если")
        {
            QVariant aCondition=calculatePart(aParameters.at(0), aErrorList, aComponent, inFrame, tableRow);

            if (aCondition.type()!=QVariant::Bool)
            {
                aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть логическое значение";
                throw "Wrong parameter";
            }

            if (aCondition.toBool())
            {
                return calculatePart(aParameters.at(1), aErrorList, aComponent, inFrame, tableRow);
            }
            else
            {
                return calculatePart(aParameters.at(2), aErrorList, aComponent, inFrame, tableRow);
            }
        }
        else
        {
            QList<QVariant> aResults;

            for (int i=0; i<aParameters.length(); i++)
            {
                aResults.append(calculatePart(aParameters.at(i), aErrorList, aComponent, inFrame, tableRow));
            }

            if (aFunction=="Сумма_чисел")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return aArg1+aArg2;
            }
            else
            if (aFunction=="Разность_чисел")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return aArg1-aArg2;
            }
            else
            if (aFunction=="Умножение")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return aArg1*aArg2;
            }
            else
            if (aFunction=="Деление")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                if (aArg2==0)
                {
                    aComponent->calculationError="Деление на ноль";
                    throw "Zero division";
                }

                return aArg1/aArg2;
            }
            else
            if (aFunction=="Степень")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return pow(aArg1, aArg2);
            }
            else
            if (aFunction=="Число_Больше")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return aArg1>aArg2;
            }
            else
            if (aFunction=="Число_Меньше")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return aArg1<aArg2;
            }
            else
            if (aFunction=="Число_Равно")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return aArg1==aArg2;
            }
            else
            if (aFunction=="Число_Не_Равно")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return aArg1!=aArg2;
            }
            else
            if (aFunction=="Число_Больше_Равно")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return aArg1>=aArg2;
            }
            else
            if (aFunction=="Число_Меньше_Равно")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return aArg1<=aArg2;
            }
            else
            if (aFunction=="Сумма_строк")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toString()+aResults.at(1).toString();
            }
            else
            if (aFunction=="Строки_Равны")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toString()==aResults.at(1).toString();
            }
            else
            if (aFunction=="Строки_Не_Равны")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toString()!=aResults.at(1).toString();
            }
            else
            if (aFunction=="Логика_И")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть логическое значение";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть логическое значение";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toBool() && aResults.at(1).toBool();
            }
            else
            if (aFunction=="Логика_ИЛИ")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть логическое значение";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть логическое значение";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toBool() || aResults.at(1).toBool();
            }
            else
            if (aFunction=="Логика_НЕ")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть логическое значение";
                    throw "Wrong parameter";
                }

                return !aResults.at(0).toBool();
            }
            else
            if (aFunction=="Логика_Равна")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть логическое значение";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть логическое значение";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toBool() == aResults.at(1).toBool();
            }
            else
            if (aFunction=="Логика_Не_Равна")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть логическое значение";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть логическое значение";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toBool() != aResults.at(1).toBool();
            }
            else
            if (aFunction=="Дата_Больше")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate() > aResults.at(1).toDate();
            }
            else
            if (aFunction=="Дата_Меньше")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate() < aResults.at(1).toDate();
            }
            else
            if (aFunction=="Дата_Равна")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate() == aResults.at(1).toDate();
            }
            else
            if (aFunction=="Дата_Не_Равна")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate() != aResults.at(1).toDate();
            }
            else
            if (aFunction=="Дата_Больше_Равна")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate() >= aResults.at(1).toDate();
            }
            else
            if (aFunction=="Дата_Меньше_Равна")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate() <= aResults.at(1).toDate();
            }
            else
            if (aFunction=="Время_Больше")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime() > aResults.at(1).toTime();
            }
            else
            if (aFunction=="Время_Меньше")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime() < aResults.at(1).toTime();
            }
            else
            if (aFunction=="Время_Равно")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime() == aResults.at(1).toTime();
            }
            else
            if (aFunction=="Время_Не_Равно")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime() != aResults.at(1).toTime();
            }
            else
            if (aFunction=="Время_Больше_Равно")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime() >= aResults.at(1).toTime();
            }
            else
            if (aFunction=="Время_Меньше_Равно")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime() <= aResults.at(1).toTime();
            }
            else
            if (aFunction=="Число_в_Логику")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return aArg1!=0;
            }
            else
            if (aFunction=="Число_в_Строку")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return QString::number(aArg1);
            }
            else
            if (aFunction=="Логика_в_Число")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть логическое значение";
                    throw "Wrong parameter";
                }

                return (double)(aResults.at(0).toBool() ? 1 : 0);
            }
            else
            if (aFunction=="Логика_в_строку")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть логическое значение";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toBool() ? "1" : "0";
            }
            else
            if (aFunction=="Строка_в_Число")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                bool ok;
                double aValue=aResults.at(0).toString().toDouble(&ok);

                if (ok)
                {
                    return aValue;
                }

                aComponent->calculationError="Невозможно преобразовать строку \""+aResults.at(0).toString()+"\"в число";
                throw "Impossible to convert";
            }
            else
            if (aFunction=="Строка_в_Логику")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toString()!="0";
            }
            else
            if (aFunction=="Строка_в_Дату")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                QDate aDate=QDate::fromString(aResults.at(0).toString(), aResults.at(1).toString());

                if (aDate.isValid())
                {
                    return aDate;
                }

                aComponent->calculationError="Невозможно преобразовать строку \""+aResults.at(0).toString()+"\"в дату";
                throw "Impossible to convert";
            }
            else
            if (aFunction=="Строка_во_Время")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                QTime aTime=QTime::fromString(aResults.at(0).toString(), aResults.at(1).toString());

                if (aTime.isValid())
                {
                    return aTime;
                }

                aComponent->calculationError="Невозможно преобразовать строку \""+aResults.at(0).toString()+"\"во время";
                throw "Impossible to convert";
            }
            else
            if (aFunction=="Строка_в_Список")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                QStringList aList=aResults.at(0).toString().split(aResults.at(1).toString());

                return aList;
            }
            else
            if (aFunction=="Дата_в_Строку")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate().toString(aResults.at(1).toString());
            }
            else
            if (aFunction=="Время_в_Строку")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime().toString(aResults.at(1).toString());
            }
            else
            if (aFunction=="Список_в_Строку")
            {
                if (aResults.at(0).type()==QVariant::String)
                {
                    PageComponent* aVariable=variableByName(aParameters.at(0), aComponent);

                    if (
                        aVariable
                        &&
                        aVariable->inherits("VariableListFrame")
                       )
                    {
                        aResults[0]=((VariableListFrame*)aVariable)->mCalculationList;
                    }
                }

                if (aResults.at(0).type()!=QVariant::StringList)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должен быть список";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toStringList().join(aResults.at(1).toString());
            }
            else
            if (aFunction=="Это_число")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                bool ok;
                aResults.at(0).toDouble(&ok);

                return ok;
            }
            else
            if (aFunction=="Это_дата")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                QDate aDateValue=QDate::fromString(aResults.at(0).toString(), "dd.MM.yyyy");
                return aDateValue.isValid();
            }
            else
            if (aFunction=="Нули_слева")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                bool ok;
                double aArg1=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                QString aRes=aResults.at(0).toString();

                while (aRes.length()<aArg1)
                {
                    aRes.insert(0, "0");
                }

                return aRes;
            }
            else
            if (aFunction=="Случайный")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return (double)(aArg1+((double)qrand())/RAND_MAX*(aArg2-aArg1));
            }
            else
            if (aFunction=="Округление")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                for (int i=0; i<aArg2; i++)
                {
                    aArg1*=10;
                }

                aArg1=floor(aArg1);

                for (int i=0; i<aArg2; i++)
                {
                    aArg1/=10;
                }

                return aArg1;
            }
            else
            if (aFunction=="Sin")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return sin(aArg1);
            }
            else
            if (aFunction=="Cos")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return cos(aArg1);
            }
            else
            if (aFunction=="Tan")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aSinus=sin(aArg1);
                double aCosinus=cos(aArg1);

                if (aCosinus==0)
                {
                    aComponent->calculationError="Тангенс не может быть определен";
                    throw "Division by zero";
                }

                return aSinus/aCosinus;
            }
            else
            if (aFunction=="CoTan")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aSinus=sin(aArg1);
                double aCosinus=cos(aArg1);

                if (aSinus==0)
                {
                    aComponent->calculationError="Котангенс не может быть определен";
                    throw "Division by zero";
                }

                return aCosinus/aSinus;
            }
            else
            if (aFunction=="ПИ")
            {
                return M_PI;
            }
            else
            if (aFunction=="Подстрока")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                bool ok;
                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg3=aResults.at(2).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Третьим параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toString().mid((int)aArg2, (int)aArg3);
            }
            else
            if (aFunction=="Длина_текста")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toString().length());
            }
            else
            if (aFunction=="Удалить_текст")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                bool ok;
                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                double aArg3=aResults.at(2).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Третьим параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                QString aValue=aResults.at(0).toString();
                aValue.remove((int)aArg2, (int)aArg3);

                return aValue;
            }
            else
            if (aFunction=="Вставить_текст")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                bool ok;
                double aArg3=aResults.at(2).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Третьим параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                QString aValue=aResults.at(1).toString();
                aValue.insert((int)aArg3, aResults.at(0).toString());

                return aValue;
            }
            else
            if (aFunction=="Позиция_в_тексте")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(1).toString().indexOf(aResults.at(0).toString()));
            }
            else
            if (aFunction=="Пробелы_справа")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть строка";
                    throw "Wrong parameter";
                }

                bool ok;
                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                QString aRes=aResults.at(0).toString();

                while (aRes.length()<aArg2)
                {
                    aRes.append(" ");
                }

                return aRes;
            }
            else
            if (aFunction=="Сегодня")
            {
                return QDate::currentDate();
            }
            else
            if (aFunction=="День")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toDate().day());
            }
            else
            if (aFunction=="Месяц")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toDate().month());
            }
            else
            if (aFunction=="Год")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toDate().year());
            }
            else
            if (aFunction=="Дней_до")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должна быть дата";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toDate().daysTo(aResults.at(1).toDate()));
            }
            else
            if (aFunction=="Сейчас")
            {
                return QTime::currentTime();
            }
            else
            if (aFunction=="Секунда")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toTime().second());
            }
            else
            if (aFunction=="Минута")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toTime().minute());
            }
            else
            if (aFunction=="Час")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toTime().hour());
            }
            else
            if (aFunction=="Секунд_до")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть время";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toTime().secsTo(aResults.at(1).toTime()));
            }
            else
            if (aFunction=="Используемые_ГОСТы")
            {
                PageComponent* aVariable=variableByName(aParameters.at(0), aComponent);

                if (aVariable==0)
                {
                    aComponent->calculationError="Не найден компонент \""+aParameters.at(0)+"\", который используется первым параметром функции \""+aFunction+"\"";
                    throw "Wrong parameter";
                }

                if (!aVariable->inherits("VariableExtendedListFrame"))
                {
                    aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должен быть расширенный список";
                    throw "Wrong parameter";
                }

                bool ok;
                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Вторым параметром функции \""+aFunction+"\" должно быть число";
                    throw "Wrong parameter";
                }

                VariableExtendedListFrame *aFrame=(VariableExtendedListFrame*)aVariable;
                int aColumn=(int)(aArg2-1);

                if (aColumn<0 || aColumn>=aFrame->typeColumns.length())
                {
                    aComponent->calculationError="Не найден столбец №"+QString::number(aColumn+1)+" в расширенном списке \""+aParameters.at(0)+"\" в функции \""+aFunction+"\"";
                    throw "Wrong parameter";
                }

                QString res="!!! ТУТ СПИСОК СТАНДАРТОВ !!!";

                QStringList aPUEList;
                QStringList aGOSTList;
                QStringList aGOSTRList;
                QStringList aRDList;
                QStringList aSOList;
                QStringList aSPList;
                QStringList aTSNList;

                QVariantList rowValues=aResults.at(0).toList();

                for (int i=0; i<rowValues.length(); i++)
                {
                    QString aOneLine=rowValues.at(i).toList().at(aColumn).toString().simplified();
                    QString aLocation="строка № "+QString::number(i+1)+", столбец № "+QString::number(aColumn+1)+" в таблице \""+aFrame->name()+"\"";

                    do
                    {
                        if (aOneLine=="")
                        {
                            break;
                        }

                        QString aOriginalLine=aOneLine;

                        if (aOneLine.startsWith("ПУЭ"))
                        {
                            QString aFormat="Формат написания стандарта: ПУЭ [:] {X.X.Z [-X.X.Z] ,}";

                            aOneLine.remove(0, 3);

                            if (aOneLine.startsWith(" "))
                            {
                                aOneLine.remove(0, 1);
                            }

                            if (aOneLine.startsWith(":"))
                            {
                                aOneLine.remove(0, 1);
                            }

                            if (aOneLine.startsWith(" "))
                            {
                                aOneLine.remove(0, 1);
                            }

                            if (aOneLine=="")
                            {
                                aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                                throw "Wrong format";
                            }

                            do
                            {
                                if (
                                    aOneLine==""
                                    ||
                                    aOneLine.startsWith("ПУЭ")
                                    ||
                                    aOneLine.startsWith("ГОСТ")
                                    ||
                                    aOneLine.startsWith("РД")
                                    ||
                                    aOneLine.startsWith("СО")
                                    ||
                                    aOneLine.startsWith("СП")
                                    ||
                                    aOneLine.startsWith("ТСН")
                                   )
                                {
                                    break;
                                }

                                QString aPart1;

                                while (aOneLine.length()>0 && aOneLine.at(0).isDigit())
                                {
                                    aPart1.append(aOneLine.at(0));
                                    aOneLine.remove(0, 1);
                                }

                                if (aPart1=="")
                                {
                                    aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                                    throw "Wrong format";
                                }

                                if (aOneLine.startsWith(" "))
                                {
                                    aOneLine.remove(0, 1);
                                }

                                if (!aOneLine.startsWith("."))
                                {
                                    aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                                    throw "Wrong format";
                                }

                                aOneLine.remove(0, 1);

                                if (aOneLine.startsWith(" "))
                                {
                                    aOneLine.remove(0, 1);
                                }

                                QString aPart2;

                                while (aOneLine.length()>0 && aOneLine.at(0).isDigit())
                                {
                                    aPart2.append(aOneLine.at(0));
                                    aOneLine.remove(0, 1);
                                }

                                if (aPart2=="")
                                {
                                    aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                                    throw "Wrong format";
                                }

                                if (aOneLine.startsWith(" "))
                                {
                                    aOneLine.remove(0, 1);
                                }

                                if (!aOneLine.startsWith("."))
                                {
                                    aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                                    throw "Wrong format";
                                }

                                aOneLine.remove(0, 1);

                                if (aOneLine.startsWith(" "))
                                {
                                    aOneLine.remove(0, 1);
                                }

                                QString aPart3;

                                while (aOneLine.length()>0 && aOneLine.at(0).isDigit())
                                {
                                    aPart3.append(aOneLine.at(0));
                                    aOneLine.remove(0, 1);
                                }

                                if (aPart3=="")
                                {
                                    aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                                    throw "Wrong format";
                                }

                                if (aOneLine.startsWith(" "))
                                {
                                    aOneLine.remove(0, 1);
                                }

                                if (aOneLine.startsWith("("))
                                {
                                    aOneLine.remove(0, 1);

                                    int aBracketCount=1;

                                    while (aOneLine.length()>0 && aBracketCount>0)
                                    {
                                        if (aOneLine.startsWith("("))
                                        {
                                            aBracketCount++;
                                        }
                                        else
                                        if (aOneLine.startsWith(")"))
                                        {
                                            aBracketCount--;
                                        }

                                        aOneLine.remove(0, 1);
                                    }

                                    if (aBracketCount>0)
                                    {
                                        aComponent->calculationError="Неверное количество скобок. "+aFormat+" "+aLocation+" => "+aOriginalLine;
                                        throw "Wrong format";
                                    }

                                    if (aOneLine.startsWith(" "))
                                    {
                                        aOneLine.remove(0, 1);
                                    }
                                }

                                if (aOneLine.startsWith("-"))
                                {
                                    aOneLine.remove(0, 1);

                                    if (aOneLine.startsWith(" "))
                                    {
                                        aOneLine.remove(0, 1);
                                    }

                                    QString aPart4;

                                    while (aOneLine.length()>0 && aOneLine.at(0).isDigit())
                                    {
                                        aPart4.append(aOneLine.at(0));
                                        aOneLine.remove(0, 1);
                                    }

                                    if (aPart4=="")
                                    {
                                        aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                                        throw "Wrong format";
                                    }

                                    if (aOneLine.startsWith(" "))
                                    {
                                        aOneLine.remove(0, 1);
                                    }

                                    if (!aOneLine.startsWith("."))
                                    {
                                        aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                                        throw "Wrong format";
                                    }

                                    aOneLine.remove(0, 1);

                                    if (aOneLine.startsWith(" "))
                                    {
                                        aOneLine.remove(0, 1);
                                    }

                                    QString aPart5;

                                    while (aOneLine.length()>0 && aOneLine.at(0).isDigit())
                                    {
                                        aPart5.append(aOneLine.at(0));
                                        aOneLine.remove(0, 1);
                                    }

                                    if (aPart5=="")
                                    {
                                        aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                                        throw "Wrong format";
                                    }

                                    if (aOneLine.startsWith(" "))
                                    {
                                        aOneLine.remove(0, 1);
                                    }

                                    if (!aOneLine.startsWith("."))
                                    {
                                        aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                                        throw "Wrong format";
                                    }

                                    aOneLine.remove(0, 1);

                                    if (aOneLine.startsWith(" "))
                                    {
                                        aOneLine.remove(0, 1);
                                    }

                                    QString aPart6;

                                    while (aOneLine.length()>0 && aOneLine.at(0).isDigit())
                                    {
                                        aPart6.append(aOneLine.at(0));
                                        aOneLine.remove(0, 1);
                                    }

                                    if (aPart6=="")
                                    {
                                        aComponent->calculationError=aFormat+" "+aLocation+" => "+aOriginalLine;
                                        throw "Wrong format";
                                    }

                                    if (aOneLine.startsWith(" "))
                                    {
                                        aOneLine.remove(0, 1);
                                    }

                                    if (aOneLine.startsWith("("))
                                    {
                                        aOneLine.remove(0, 1);

                                        int aBracketCount=1;

                                        while (aOneLine.length()>0 && aBracketCount>0)
                                        {
                                            if (aOneLine.startsWith("("))
                                            {
                                                aBracketCount++;
                                            }
                                            else
                                            if (aOneLine.startsWith(")"))
                                            {
                                                aBracketCount--;
                                            }

                                            aOneLine.remove(0, 1);
                                        }

                                        if (aBracketCount>0)
                                        {
                                            aComponent->calculationError="Неверное количество скобок. "+aFormat+" "+aLocation+" => "+aOriginalLine;
                                            throw "Wrong format";
                                        }

                                        if (aOneLine.startsWith(" "))
                                        {
                                            aOneLine.remove(0, 1);
                                        }
                                    }

                                    int aStart=aPart2.toInt();
                                    int aEnd=aPart5.toInt();

                                    if (aPart1!=aPart4 || aStart>aEnd)
                                    {
                                        aComponent->calculationError="Неверный диапазон ("+aPart1+"."+aPart2+"."+aPart3+"-"+aPart4+"."+aPart5+"."+aPart6+"). "+aFormat+" "+aLocation+" => "+aOriginalLine;
                                        throw "Wrong format";
                                    }

                                    for (int j=aStart; j<=aEnd; j++)
                                    {
                                        if (!aPUEList.contains(aPart1+"."+QString::number(j)))
                                        {
                                            aPUEList.append(aPart1+"."+QString::number(j));
                                        }
                                    }
                                }
                                else
                                {
                                    if (!aPUEList.contains(aPart1+"."+aPart2))
                                    {
                                        aPUEList.append(aPart1+"."+aPart2);
                                    }
                                }

                                if (aOneLine.startsWith(",") || aOneLine.startsWith("."))
                                {
                                    aOneLine.remove(0, 1);
                                }

                                if (aOneLine.startsWith(" "))
                                {
                                    aOneLine.remove(0, 1);
                                }
                            } while (true);
                        }
                        else
                        if (aOneLine.startsWith("ГОСТ Р"))
                        {
                            QList<EGOSTDelimiter> delimiters;

                            delimiters.append(gdMultiPoint);
                            delimiters.append(gdMinus);

                            readGOST(aComponent, aOneLine, aGOSTRList, "ГОСТ Р", "ГОСТ Р X [{.X}] - X", aLocation, delimiters);
                        }
                        else
                        if (aOneLine.startsWith("ГОСТ"))
                        {
                            QList<EGOSTDelimiter> delimiters;

                            delimiters.append(gdMultiPoint);
                            delimiters.append(gdMinus);

                            readGOST(aComponent, aOneLine, aGOSTList, "ГОСТ", "ГОСТ X [{.X}] - X", aLocation, delimiters);
                        }
                        else
                        if (aOneLine.startsWith("РД"))
                        {
                            QList<EGOSTDelimiter> delimiters;

                            delimiters.append(gdPoint);
                            delimiters.append(gdPoint);
                            delimiters.append(gdMinus);

                            readGOST(aComponent, aOneLine, aRDList, "РД", "РД X.X.X-X", aLocation, delimiters);
                        }
                        else
                        if (aOneLine.startsWith("СО"))
                        {
                            QList<EGOSTDelimiter> delimiters;

                            delimiters.append(gdMinus);
                            delimiters.append(gdPoint);
                            delimiters.append(gdPoint);
                            delimiters.append(gdMinus);

                            readGOST(aComponent, aOneLine, aSOList, "СО", "СО X-X.X.X-X", aLocation, delimiters);
                        }
                        else
                        if (aOneLine.startsWith("СП"))
                        {
                            QList<EGOSTDelimiter> delimiters;

                            delimiters.append(gdMinus);
                            delimiters.append(gdMinus);

                            readGOST(aComponent, aOneLine, aSPList, "СП", "СП X-X-X", aLocation, delimiters);
                        }
                        else
                        if (aOneLine.startsWith("ТСН"))
                        {
                            QList<EGOSTDelimiter> delimiters;

                            delimiters.append(gdMinus);
                            delimiters.append(gdMinus);

                            readGOST(aComponent, aOneLine, aTSNList, "ТСН", "ТСН X-X-X", aLocation, delimiters);
                        }
                        else
                        {
                            aComponent->calculationError="Неизвестный стандарт \""+aOneLine+"\". "+aLocation;
                            throw "Wrong format";
                        }
                    } while (true);
                }


                if (
                    aPUEList.length()>0
                    ||
                    aGOSTList.length()>0
                    ||
                    aGOSTRList.length()>0
                    ||
                    aRDList.length()>0
                    ||
                    aSOList.length()>0
                    ||
                    aSPList.length()>0
                    ||
                    aTSNList.length()>0
                   )
                {
                    res="";

                    aGOSTList.sort();
                    aGOSTRList.sort();
                    aRDList.sort();
                    aSOList.sort();
                    aSPList.sort();
                    aTSNList.sort();

                    if (aPUEList.length()>0)
                    {
                        aPUEList.sort();
                        res.append("ПУЭ: ");
                        res.append(aPUEList.join(", "));
                    }

                    if (res!="" && !res.endsWith(";\n"))
                    {
                        res.append(";\n");
                    }

                    res.append(aGOSTList.join(", "));

                    if (res!="" && !res.endsWith(";\n"))
                    {
                        res.append(";\n");
                    }

                    res.append(aGOSTRList.join(", "));

                    if (res!="" && !res.endsWith(";\n"))
                    {
                        res.append(";\n");
                    }

                    res.append(aRDList.join(", "));

                    if (res!="" && !res.endsWith(";\n"))
                    {
                        res.append(";\n");
                    }

                    res.append(aSOList.join(", "));

                    if (res!="" && !res.endsWith(";\n"))
                    {
                        res.append(";\n");
                    }

                    res.append(aSPList.join(", "));

                    if (res!="" && !res.endsWith(";\n"))
                    {
                        res.append(";\n");
                    }

                    res.append(aTSNList.join(", "));

                    if (res.endsWith(";\n"))
                    {
                        res.remove(res.length()-2, 2);
                    }
                }

                return res;
            }
        }
    }
    else
    {
        if (aExpression=="true")
        {
            return true;
        }
        else
        if (aExpression=="false")
        {
            return false;
        }
        else
        if (aExpression.startsWith("\"") || aExpression.startsWith("\'"))
        {
            quote=aExpression.at(0);

            for (int i=1; i<aExpression.length(); i++)
            {
                if (aExpression.at(i)==quote)
                {
                    if (i<aExpression.length()-1)
                    {
                        break;
                    }

                    aExpression.remove(i, 1);
                    aExpression.remove(0, 1);

                    aExpression.replace("\\\"", "\"");
                    aExpression.replace("\\\'", "\'");
                    aExpression.replace("\\n", "\n");
                    aExpression.replace("\\r", "\r");
                    aExpression.replace("\\t", "\t");
                    aExpression.replace("\\\\", "\\");

                    return aExpression;
                }
                else
                if (aExpression.at(i)=='\\')
                {
                    i++;
                }
            }

            aComponent->calculationError="Строка \""+aExpression+"\" должна была закончиться кавычкой "+QString(quote);
            throw "Wrong end";
        }
        else
        if (aExpression.startsWith("[") && aExpression.endsWith("]"))
        {
            if (inFrame==0)
            {
                aComponent->calculationError="Обращение к столбцу \""+aExpression+"\" доступно только для таблицы";
                throw "Column index is unavailable";
            }

            aExpression.remove(aExpression.length()-1, 1);
            aExpression.remove(0, 1);

            bool ok;
            int aColumn=aExpression.toInt(&ok);

            if (!ok)
            {
                aComponent->calculationError="Индекс столбца \"["+aExpression+"]\" должен быть числом";
                throw "Column index is not a number";
            }

            if (aColumn<1 || aColumn>inFrame->typeColumns.length())
            {
                aComponent->calculationError="Не найден "+aExpression+" столбец";
                throw "Wrong column index";
            }

            aColumn--;

            if (!inFrame->mCellResults.at(tableRow).at(aColumn).isValid())
            {
                throw "Not now. Maybe later";
            }

            return inFrame->mCellResults.at(tableRow).at(aColumn);
        }
        else
        if (aExpression.at(0).isNumber())
        {
            bool ok;
            double aDoubleValue=aExpression.toDouble(&ok);

            if (ok)
            {
                return aDoubleValue;
            }

            QDate aDateValue=QDate::fromString(aExpression, "dd.MM.yyyy");

            if (aDateValue.isValid())
            {
                return aDateValue;
            }

            QTime aTimeValue=QTime::fromString(aExpression, "h:mm:ss");

            if (aTimeValue.isValid())
            {
                return aTimeValue;
            }

            aComponent->calculationError="Невозможно преобразовать строку \""+aExpression+"\" в число";
            throw "Impossible to convert";
        }
        else
        if (aExpression.length()>1 && aExpression.at(0)=='-' && aExpression.at(1).isNumber())
        {
            bool ok;
            double aDoubleValue=aExpression.toDouble(&ok);

            if (ok)
            {
                return aDoubleValue;
            }

            aComponent->calculationError="Невозможно преобразовать строку \""+aExpression+"\" в число";
            throw "Impossible to convert";
        }
        else
        {
            QString aColumnIndex="";

            int index=aExpression.indexOf("[");

            if (index>=0)
            {
                if (!aExpression.endsWith("]"))
                {
                    aComponent->calculationError="Не найдена закрывающаяся квадратная скобка в \""+aExpression+"\"";
                    throw "Wrong column index";
                }

                aColumnIndex=aExpression.mid(index+1);
                aColumnIndex.remove(aColumnIndex.length()-1, 1);

                aExpression=aExpression.left(index);
            }

            PageComponent *aVariable=getVariableOrThrow(aExpression, aComponent);
            QVariant varResult;

            try
            {
                varResult=aVariable->calculate(aErrorList);
            }
            catch (...)
            {
                aComponent->calculationError=aVariable->calculationError;
                throw "Recursive throw";
            }

            if (aColumnIndex=="")
            {
                return varResult;
            }
            else
            {
                if (!aVariable->inherits("VariableExtendedListFrame"))
                {
                    aComponent->calculationError="Переменная \""+aExpression+"\" не является расширенным списком";
                    throw "Supported for VariableExtendedListFrame only";
                }

                bool ok;
                int aColumnID=aColumnIndex.toInt(&ok);

                if (!ok)
                {
                    aComponent->calculationError="Индекс таблицы \"["+aColumnIndex+"]\" не является числом";
                    throw "Column index is not a number";
                }

                if (aColumnID<1 || aColumnID>((VariableExtendedListFrame*)aVariable)->typeColumns.length())
                {
                    aComponent->calculationError="Не найден "+aColumnIndex+" столбец";
                    throw "Column index not found";
                }

                aColumnID--;

                QStringList aResultList;
                QVariantList rowValues=varResult.toList();

                for (int i=0; i<rowValues.length(); i++)
                {
                    QVariantList colValues=rowValues.at(i).toList();
                    QVariant aValue=colValues.at(aColumnID);

                    if (aValue.type()==QVariant::Date)
                    {
                        aResultList.append(aValue.toDate().toString("dd.MM.yyyy"));
                    }
                    else
                    if (aValue.type()==QVariant::Time)
                    {
                        aResultList.append(aValue.toTime().toString("hh:mm:ss"));
                    }
                    else
                    if (aValue.type()==QVariant::Bool)
                    {
                        if (aValue.toBool())
                        {
                            aResultList.append("1");
                        }
                        else
                        {
                            aResultList.append("0");
                        }
                    }
                    else
                    {
                        aResultList.append(aValue.toString());
                    }
                }

                return aResultList;
            }
        }
    }

    return 0;
}

void replaceLinksInText(QTextEdit *aTextEdit, PageComponent *aPage)
{
    QTextCursor aCursor=aTextEdit->textCursor();

    QTextDocument *doc=aTextEdit->document();

    for (QTextBlock it=doc->begin(); it!=doc->end(); it=it.next())
    {
        aCursor.setPosition(it.position());

        while (
               !aCursor.atEnd()
               &&
               it.contains(aCursor.position())
              )
        {
            aCursor.setPosition(aCursor.position()+1, QTextCursor::KeepAnchor);

            if (aCursor.selectedText()=="[")
            {
                while (
                       !aCursor.atEnd()
                       &&
                       it.contains(aCursor.position())
                       &&
                       !aCursor.selectedText().endsWith("]")
                      )
                {
                    aCursor.setPosition(aCursor.position()+1, QTextCursor::KeepAnchor);
                }

                QString aSelection=aCursor.selectedText();

                if (aSelection.endsWith("]"))
                {
                    aSelection.remove(aSelection.length()-1, 1);
                    aSelection.remove(0, 1);

                    PageComponent* aComponent=variableByName(aSelection, aPage);

                    if (aComponent)
                    {
                        aCursor.insertText(variantToText(aComponent->calculate()));
                    }
                }
            }

            aCursor.setPosition(aCursor.position());
        }
    }
}

QString variantToText(const QVariant &aVariant)
{
    switch (aVariant.type())
    {
        case QVariant::Double: return QString::number(aVariant.toDouble());
        case QVariant::String: return aVariant.toString();
        case QVariant::Bool:
        {
            if (aVariant.toBool())
            {
                return "1";
            }
            else
            {
                return "0";
            }
        }
        case QVariant::Date: return aVariant.toDate().toString("dd.MM.yyyy");
        case QVariant::Time: return aVariant.toTime().toString("hh:mm:ss");
        default: return "!!!NOT_SUPPORTED!!!";
    }

    return "!!!NOT_SUPPORTED!!!";
}
