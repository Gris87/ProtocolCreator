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
                aComponent->calculationError="�� ������� ���������� ���������� \""+aVariableName+"\"";
            }
            else
            {
                aComponent->calculationError="�� ������� ���������� \""+aVariableName+"\" � ������� \""+aSection+"\"";
            }
        }
        else
        {
            aComponent->calculationError="�� ������� ���������� \""+aVariableName+"\"";
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
            else
            if (aFunction=="����_������")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate() > aResults.at(1).toDate();
            }
            else
            if (aFunction=="����_������")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate() < aResults.at(1).toDate();
            }
            else
            if (aFunction=="����_�����")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate() == aResults.at(1).toDate();
            }
            else
            if (aFunction=="����_��_�����")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate() != aResults.at(1).toDate();
            }
            else
            if (aFunction=="����_������_�����")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate() >= aResults.at(1).toDate();
            }
            else
            if (aFunction=="����_������_�����")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate() <= aResults.at(1).toDate();
            }
            else
            if (aFunction=="�����_������")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime() > aResults.at(1).toTime();
            }
            else
            if (aFunction=="�����_������")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime() < aResults.at(1).toTime();
            }
            else
            if (aFunction=="�����_�����")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime() == aResults.at(1).toTime();
            }
            else
            if (aFunction=="�����_��_�����")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime() != aResults.at(1).toTime();
            }
            else
            if (aFunction=="�����_������_�����")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime() >= aResults.at(1).toTime();
            }
            else
            if (aFunction=="�����_������_�����")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime() <= aResults.at(1).toTime();
            }
            else
            if (aFunction=="�����_�_������")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aArg1!=0;
            }
            else
            if (aFunction=="�����_�_������")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return QString::number(aArg1);
            }
            else
            if (aFunction=="������_�_�����")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
                    throw "Wrong parameter";
                }

                return (double)(aResults.at(0).toBool() ? 1 : 0);
            }
            else
            if (aFunction=="������_�_������")
            {
                if (aResults.at(0).type()!=QVariant::Bool)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toBool() ? "1" : "0";
            }
            else
            if (aFunction=="������_�_�����")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                bool ok;
                double aValue=aResults.at(0).toString().toDouble(&ok);

                if (ok)
                {
                    return aValue;
                }

                aComponent->calculationError="���������� ������������� ������ \""+aResults.at(0).toString()+"\"� �����";
                throw "Impossible to convert";
            }
            else
            if (aFunction=="������_�_������")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toString()!="0";
            }
            else
            if (aFunction=="������_�_����")
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

                QDate aDate=QDate::fromString(aResults.at(0).toString(), aResults.at(1).toString());

                if (aDate.isValid())
                {
                    return aDate;
                }

                aComponent->calculationError="���������� ������������� ������ \""+aResults.at(0).toString()+"\"� ����";
                throw "Impossible to convert";
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

                QTime aTime=QTime::fromString(aResults.at(0).toString(), aResults.at(1).toString());

                if (aTime.isValid())
                {
                    return aTime;
                }

                aComponent->calculationError="���������� ������������� ������ \""+aResults.at(0).toString()+"\"�� �����";
                throw "Impossible to convert";
            }
            else
            if (aFunction=="������_�_������")
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

                QStringList aList=aResults.at(0).toString().split(aResults.at(1).toString());

                return aList;
            }
            else
            if (aFunction=="����_�_������")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toDate().toString(aResults.at(1).toString());
            }
            else
            if (aFunction=="�����_�_������")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toTime().toString(aResults.at(1).toString());
            }
            else
            if (aFunction=="������_�_������")
            {
                if (aResults.at(0).type()!=QVariant::StringList)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                if (aResults.at(1).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toStringList().join(aResults.at(1).toString());
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

                return (double)(aArg1+((double)qrand())/RAND_MAX*(aArg2-aArg1));
            }
            else
            if (aFunction=="����������")
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
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
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
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
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
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aSinus=sin(aArg1);
                double aCosinus=cos(aArg1);

                if (aCosinus==0)
                {
                    aComponent->calculationError="������� �� ����� ���� ���������";
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
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aSinus=sin(aArg1);
                double aCosinus=cos(aArg1);

                if (aSinus==0)
                {
                    aComponent->calculationError="��������� �� ����� ���� ���������";
                    throw "Division by zero";
                }

                return aCosinus/aSinus;
            }
            else
            if (aFunction=="��")
            {
                return M_PI;
            }
            else
            if (aFunction=="���������")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                bool ok;
                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg3=aResults.at(2).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������� ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return aResults.at(0).toString().mid((int)aArg2, (int)aArg3);
            }
            else
            if (aFunction=="�����_������")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toString().length());
            }
            else
            if (aFunction=="�������_�����")
            {
                if (aResults.at(0).type()!=QVariant::String)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ������";
                    throw "Wrong parameter";
                }

                bool ok;
                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg3=aResults.at(2).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������� ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                QString aValue=aResults.at(0).toString();
                aValue.remove((int)aArg2, (int)aArg3);

                return aValue;
            }
            else
            if (aFunction=="��������_�����")
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

                bool ok;
                double aArg3=aResults.at(2).toDouble(&ok);

                if (!ok)
                {
                    aComponent->calculationError="������� ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                QString aValue=aResults.at(1).toString();
                aValue.insert((int)aArg3, aResults.at(0).toString());

                return aValue;
            }
            else
            if (aFunction=="�������_�_������")
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

                return ((double)aResults.at(1).toString().indexOf(aResults.at(0).toString()));
            }
            else
            if (aFunction=="�������")
            {
                return QDate::currentDate();
            }
            else
            if (aFunction=="����")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toDate().day());
            }
            else
            if (aFunction=="�����")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toDate().month());
            }
            else
            if (aFunction=="���")
            {
                if (aResults.at(0).type()!=QVariant::Date)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ����";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toDate().year());
            }
            else
            if (aFunction=="������")
            {
                return QTime::currentTime();
            }
            else
            if (aFunction=="�������")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toTime().second());
            }
            else
            if (aFunction=="������")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                return ((double)aResults.at(0).toTime().minute());
            }
            else
            if (aFunction=="���")
            {
                if (aResults.at(0).type()!=QVariant::Time)
                {
                    aComponent->calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
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
