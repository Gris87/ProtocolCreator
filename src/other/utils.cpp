#include "src/other/global.h"

void copyVariable(PageComponent *aComponent)
{
    PageSelectionDialog dialog(mainWindow);

    dialog.ui->titleLabel->setText("�������� ������, � ������� ����� ����������� ����������:");

    if (dialog.exec())
    {
        int aRow=dialog.ui->pagesListWidget->currentRow();

        if (aRow>0 && mainWindow->ui->pagesTabWidget->widget(aRow-1)==mainWindow->contentPage)
        {
            QMessageBox::information(mainWindow, protocolCreatorVersion, "������ ��������� ���������� � ����������");
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
            !(aName.at(i)>='�' && aName.at(i)<='�')
            &&
            !(aName.at(i)>='�' && aName.at(i)<='�')
            &&
            !(aName.at(i)>='0' && aName.at(i)<='9')
            &&
            aName.at(i)!='_'
           )
        {
            aErrorList.append("Error: ������������ ������ \""+QString(aName.at(i))+"\" � �����");
        }
    }

    if (aName.length()>0 && aName.at(0)>='0' && aName.at(0)<='9')
    {
        aErrorList.append("Error: ��� �� ����� ���������� � �����");
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
                    aComponent->calculationError="����������� ������ \\"+QString(aExpression.at(i+1))+" ��������� ��� ������";
                    throw "Special symbol outside of string";
                }
                else
                {
                    aComponent->calculationError="�� ���������� ��������";
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
                    aComponent->calculationError="�� ���������� ������������� ������ \"(\" ��� ������������� \")\"";
                    throw "No open bracket";
                }

                if (bracketCount==0 && i<aExpression.length()-1)
                {
                    aComponent->calculationError="��������� ������ ���� ���������� ������������� ������� \")\", ������ ������� ����� \""+aExpression.mid(i+1)+"\"";
                    throw "Wrong end";
                }
            }
        }
    }

    if (bracketCount>0)
    {
        aComponent->calculationError="�� ���������� ������������� ������ \")\" ��� ������������� \"(\"";
        throw "No close bracket";
    }

    if (quote!='0')
    {
        aComponent->calculationError="�� ������� ������������� ������� "+QString(quote);
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
            aComponent->calculationError="����� ������������� ������� \"(\" ������ ���� ������� �������";
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
                            aComponent->calculationError="������� \""+aFunction+"\" �� ������ ��������� ����������";
                        }
                        else
                        if (paramCount==1)
                        {
                            aComponent->calculationError="������� \""+aFunction+"\" ������ ��������� 1 ��������";
                        }
                        else
                        if (paramCount<5)
                        {
                            aComponent->calculationError="������� \""+aFunction+"\" ������ ��������� "+QString::number(paramCount)+" ��������a";
                        }
                        else
                        {
                            aComponent->calculationError="������� \""+aFunction+"\" ������ ��������� "+QString::number(paramCount)+" ����������";
                        }

                        throw "Wrong parameters count";
                    }

                    break;
                }
            }
        }

        if (!good)
        {
            aComponent->calculationError="����������� ������� \""+aFunction+"\"";
            throw "Unknown function";
        }

        if (aFunction=="����")
        {
            QVariant aCondition=calculatePart(aParameters.at(0), aComponent);

            if (aCondition.type()!=QVariant::Bool)
            {
                aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
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

            if (aFunction=="�����_�����")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aArg1+aArg2;
            }
            else
            if (aFunction=="��������_�����")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aArg1-aArg2;
            }
            else
            if (aFunction=="���������")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aArg1*aArg2;
            }
            else
            if (aFunction=="�������")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                if (aArg2==0)
                {
                    aComponent->calculationError="������� �� ����";
                    throw "Zero division";
                }

                return aArg1/aArg2;
            }
            else
            if (aFunction=="�������")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return pow(aArg1, aArg2);
            }
            else
            if (aFunction=="�����_������")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aArg1>aArg2;
            }
            else
            if (aFunction=="�����_������")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aArg1<aArg2;
            }
            else
            if (aFunction=="�����_�����")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aArg1==aArg2;
            }
            else
            if (aFunction=="�����_��_�����")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aArg1!=aArg2;
            }
            else
            if (aFunction=="�����_������_�����")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aArg1>=aArg2;
            }
            else
            if (aFunction=="�����_������_�����")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aArg1<=aArg2;
            }
            else
            if (aFunction=="�����_�����")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toString()+aResults.at(1).toString();
            }
            else
            if (aFunction=="������_�����")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toString()==aResults.at(1).toString();
            }
            else
            if (aFunction=="������_��_�����")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toString()!=aResults.at(1).toString();
            }
            else
            if (aFunction=="������_�")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toBool() && aResults.at(1).toBool();
            }
            else
            if (aFunction=="������_���")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toBool() || aResults.at(1).toBool();
            }
            else
            if (aFunction=="������_��")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
                    throw "Wrong parameter";
                }

                return !aResults.at(0).toBool();
            }
            else
            if (aFunction=="������_�����")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toBool() == aResults.at(1).toBool();
            }
            else
            if (aFunction=="������_��_�����")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
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

            aComponent->calculationError="������ \""+aExpression+"\" ������ ���� ����������� �������� "+QString(quote);
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

            aComponent->calculationError="���������� ������������� ������ \""+aExpression+"\" � �����";
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

            aComponent->calculationError="���������� ������������� ������ \""+aExpression+"\" � �����";
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
                        aComponent->calculationError="�� ������� ���������� ���������� \""+aExpression+"\"";
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
                        aComponent->calculationError="�� ������ ������ \""+aSection+"\"";
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
                        aComponent->calculationError="�� ������� ���������� \""+aExpression+"\" � ������� \""+aSection+"\"";
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
                aComponent->calculationError="�� ������� ���������� \""+aExpression+"\"";
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
