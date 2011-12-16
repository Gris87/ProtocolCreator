#include "src/other/global.h"

VariableExpressionFrame::VariableExpressionFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableExpressionFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("���������");
    ui->varNameEdit->setText("Expression");
}

VariableExpressionFrame::~VariableExpressionFrame()
{
    delete ui;
}

QString VariableExpressionFrame::name()
{
    return ui->nameEdit->text();
}

QString VariableExpressionFrame::variableName()
{
    return ui->varNameEdit->text();
}

void VariableExpressionFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarExpression");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    bool aEdit=isEditable();

    aStream << QString("Edit");
    aStream << aEdit;

    bool aLock=!ui->valueEdit->isEnabled();

    aStream << QString("Lock");
    aStream << aLock;

    QString aValue=ui->valueEdit->text();

    aStream << QString("Value");
    aStream << aValue;

    aStream << QString("VarEnd");
}

void VariableExpressionFrame::loadFromStream(QDataStream &aStream)
{
    QString aMagicWord;

    while (!aStream.atEnd())
    {
        aStream >> aMagicWord;

        if (aMagicWord=="Name")
        {
            aStream >> aMagicWord;
            ui->nameEdit->setText(aMagicWord);
        }
        else
        if (aMagicWord=="VarName")
        {
            aStream >> aMagicWord;
            ui->varNameEdit->setText(aMagicWord);
        }
        else
        if (aMagicWord=="Edit")
        {
            bool aEdit;

            aStream >> aEdit;
            ui->editButton->setFlat(!aEdit);
        }
        else
        if (aMagicWord=="Lock")
        {
            bool aLock;

            aStream >> aLock;
            ui->valueEdit->setEnabled(!aLock);

            updateLock();
        }
        else
        if (aMagicWord=="Value")
        {
            QString aValue;

            aStream >> aValue;
            ui->valueEdit->setText(aValue);
        }
        else
        if (aMagicWord=="VarEnd")
        {
            break;
        }
    }
}

void VariableExpressionFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
}

void VariableExpressionFrame::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    ui->upButton->setEnabled(aUpEnabled);
    ui->downButton->setEnabled(aDownEnabled);
}

void VariableExpressionFrame::on_upButton_clicked()
{
    emit upPressed(this);
}

void VariableExpressionFrame::on_downButton_clicked()
{
    emit downPressed(this);
}

void VariableExpressionFrame::on_copyButton_clicked()
{
    emit copyPressed(this);
}

void VariableExpressionFrame::on_deleteButton_clicked()
{
    emit deletePressed(this);
}

void VariableExpressionFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->valueLabel->setText(aName);
}

void VariableExpressionFrame::on_lockButton_clicked()
{
    ui->valueEdit->setEnabled(!ui->valueEdit->isEnabled());

    updateLock();
}

void VariableExpressionFrame::updateLock()
{
    ui->functionButton->setEnabled(ui->valueEdit->isEnabled());

    if (ui->valueEdit->isEnabled())
    {
        ui->lockButton->setIcon(QIcon(":/images/Unlock.png"));
    }
    else
    {
        ui->lockButton->setIcon(QIcon(":/images/Lock.png"));
    }
}

void VariableExpressionFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool VariableExpressionFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

void VariableExpressionFrame::on_functionButton_clicked()
{
    FunctionDialog dialog(this);
    dialog.ui->expressionEdit->setText(ui->valueEdit->text());

    if (dialog.exec())
    {
        ui->valueEdit->setText(dialog.ui->expressionEdit->text());
    }
}

QVariant VariableExpressionFrame::calculatePart(QString aExpression)
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
                    calculationError="����������� ������ \\"+QString(aExpression.at(i+1))+" ��������� ��� ������";
                    throw "Special symbol outside of string";
                }
                else
                {
                    calculationError="�� ���������� ��������";
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
                    calculationError="�� ���������� ������������� ������ \"(\" ��� ������������� \")\"";
                    throw "No open bracket";
                }

                if (bracketCount==0 && i<aExpression.length()-1)
                {
                    calculationError="��������� ������ ���� ���������� ������������� ������� \")\"";
                    throw "Wrong end";
                }
            }
        }
    }

    if (bracketCount>0)
    {
        calculationError="�� ���������� ������������� ������ \")\" ��� ������������� \"(\"";
        throw "No close bracket";
    }

    if (quote!='0')
    {
        calculationError="�� ������� ������������� ������� "+QString(quote);
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
            calculationError="����� ������������� ������� \"(\" ������ ���� ������� �������";
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
                    i=0;
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
                            calculationError="������� \""+aFunction+"\" �� ������ ��������� ����������";
                        }
                        else
                        if (paramCount==1)
                        {
                            calculationError="������� \""+aFunction+"\" ������ ��������� 1 ��������";
                        }
                        else
                        if (paramCount<5)
                        {
                            calculationError="������� \""+aFunction+"\" ������ ��������� "+QString::number(paramCount)+" ��������a";
                        }
                        else
                        {
                            calculationError="������� \""+aFunction+"\" ������ ��������� "+QString::number(paramCount)+" ����������";
                        }

                        throw "Wrong parameters count";
                    }

                    break;
                }
            }
        }

        if (!good)
        {
            calculationError="����������� ������� \""+aFunction+"\"";
            throw "Unknown function";
        }

        if (aFunction=="����")
        {
            QVariant aCondition=calculatePart(aParameters.at(0));

            if (aCondition.type()!=QVariant::Bool)
            {
                calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� ���������� ��������";
                throw "Wrong parameter";
            }

            if (aCondition.toBool())
            {
                return calculatePart(aParameters.at(1));
            }
            else
            {
                return calculatePart(aParameters.at(2));
            }
        }
        else
        {
            QList<QVariant> aResults;

            for (int i=0; i<aParameters.length(); i++)
            {
                aResults.append(calculatePart(aParameters.at(i)));
            }

            if (aFunction=="�����_�����")
            {
                bool ok;
                double aArg1=aResults.at(0).toDouble(&ok);

                if (!ok)
                {
                    calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
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
                    calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
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
                    calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
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
                    calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                double aArg2=aResults.at(1).toDouble(&ok);

                if (!ok)
                {
                    calculationError="������ ���������� ������� \""+aFunction+"\" ������ ���� �����";
                    throw "Wrong parameter";
                }

                if (aArg2==0)
                {
                    calculationError="������� �� ����";
                    throw "Zero division";
                }

                return aArg1/aArg2;
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

            calculationError="������ \""+aExpression+"\" ������ ���� ����������� �������� "+QString(quote);
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

            calculationError="���������� ������������� ������ \""+aExpression+"\" � �����";
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

            calculationError="���������� ������������� ������ \""+aExpression+"\" � �����";
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
                        calculationError="�� ������� ���������� ���������� \""+aExpression+"\"";
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
                        calculationError="�� ������ ������ \""+aSection+"\"";
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
                        calculationError="�� ������� ���������� \""+aExpression+"\" � ������� \""+aSection+"\"";
                        throw "Variable not found";
                    }
                }
            }
            else
            {
                if (superParent!=globalDialog)
                {
                    PageFrame *aPage=(PageFrame*)superParent;

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
                calculationError="�� ������� ���������� \""+aExpression+"\"";
                throw "Variable not found";
            }

            try
            {
                return aVariable->calculate();
            }
            catch (...)
            {
                calculationError=aVariable->calculationError;
                throw "Recursive throw";
            }
        }
    }

    return 0;
}

QVariant VariableExpressionFrame::calculate()
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate();

    calculationResult=calculatePart(ui->valueEdit->text());

    isWasCalculated=true;
    isInCalculation=false;

    return calculationResult;
}
