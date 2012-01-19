#include "src/other/global.h"

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
        if (aComponent->superParent!=globalDialog)
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

QVariant calculatePart(QString aExpression, PageComponent *aComponent)
{
    qDebug()<<aExpression;

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
            QVariant aCondition=calculatePart(aParameters.at(0), aComponent);

            if (aCondition.type()!=QVariant::Bool)
            {
                aComponent->calculationError="Первым параметром функции \""+aFunction+"\" должно быть логическое значение";
                throw "Wrong parameter";
            }

            if (aCondition.toBool())
            {
                return calculatePart(aParameters.at(1), aComponent);
            }
            else
            {
                return calculatePart(aParameters.at(2), aComponent);
            }
        }
        else
        {
            QList<QVariant> aResults;

            for (int i=0; i<aParameters.length(); i++)
            {
                aResults.append(calculatePart(aParameters.at(i), aComponent));
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

            QTime aTimeValue=QTime::fromString(aExpression, "hh:mm:ss");

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
            PageComponent *aVariable=getVariableOrThrow(aExpression, aComponent);

            try
            {
                return aVariable->calculate();
            }
            catch (...)
            {
                aComponent->calculationError=aVariable->calculationError;
                throw "Recursive throw";
            }
        }
    }

    return 0;
}
