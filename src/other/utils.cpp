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

            ((VariableExtendedListFrame*)aVariable)->ui->useCheckBox->setVisible(false);
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
            PageComponent *aVariable=0;
            int dopIndex=aExpression.indexOf(".");

            if (dopIndex>=0)
            {
                QString aSection=aExpression.left(dopIndex);
                aExpression.remove(0, dopIndex+1);

                if (aSection=="Global")
                {
                    for (int i=0; i<globalDialog->variables.length(); i++)
                    {
                        if (globalDialog->variables.at(i)->variableName()==aExpression)
                        {
                            aVariable=globalDialog->variables.at(i);
                            break;
                        }
                    }

                    if (aVariable==0)
                    {
                        aComponent->calculationError="Не найдена глобальная переменная \""+aExpression+"\"";
                        throw "Variable not found";
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

                    if (aPage==0)
                    {
                        aComponent->calculationError="Не найден раздел \""+aSection+"\"";
                        throw "Page not found";
                    }

                    for (int i=0; i<aPage->variables.length(); i++)
                    {
                        if (aPage->variables.at(i)->variableName()==aExpression)
                        {
                            aVariable=aPage->variables.at(i);
                            break;
                        }
                    }

                    if (aVariable==0)
                    {
                        for (int i=0; i<aPage->components.length(); i++)
                        {
                            if (aPage->components.at(i)->variableName()==aExpression)
                            {
                                aVariable=aPage->components.at(i);
                                break;
                            }
                        }
                    }

                    if (aVariable==0)
                    {
                        aComponent->calculationError="Не найдена переменная \""+aExpression+"\" в разделе \""+aSection+"\"";
                        throw "Variable not found";
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
                        if (aPage->variables.at(i)->variableName()==aExpression)
                        {
                            aVariable=aPage->variables.at(i);
                            break;
                        }
                    }

                    if (aVariable==0)
                    {
                        for (int i=0; i<aPage->components.length(); i++)
                        {
                            if (aPage->components.at(i)->variableName()==aExpression)
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
                        if (globalDialog->variables.at(i)->variableName()==aExpression)
                        {
                            aVariable=globalDialog->variables.at(i);
                            break;
                        }
                    }
                }
            }

            if (aVariable==0)
            {
                aComponent->calculationError="Не найдена переменная \""+aExpression+"\"";
                throw "Variable not found";
            }

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
