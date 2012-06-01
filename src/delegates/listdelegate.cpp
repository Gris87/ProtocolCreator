#include "src/other/global.h"

ListDelegate::ListDelegate(QString aLink, QObject *parent)
    : QItemDelegate(parent)
{
    mLink=aLink;
}

QWidget *ListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);

    editor->setEditable(true);
    editor->setInsertPolicy(QComboBox::NoInsert);

    QString aLink=mLink;

    int str_index=aLink.indexOf(".");

    if (str_index>=0)
    {
        QString aSectionName=aLink.left(str_index);
        aLink=aLink.mid(str_index+1);

        QString aVarName;
        QString aColumnIndex="";
        str_index=aLink.indexOf("[");

        if (str_index>=0)
        {
            aVarName=aLink.left(str_index);
            aLink=aLink.mid(str_index+1);
            aLink.remove(aLink.length()-1, 1);

            aColumnIndex=aLink;
        }
        else
        {
            aVarName=aLink;
        }

        VariableExtendedListFrame* aExtFrame=0;
        VariableListFrame* aFrame=0;

        if (aSectionName=="Global")
        {
            for (int i=0; i<globalDialog->variables.length(); i++)
            {
                if (globalDialog->variables.at(i)->variableName()==aVarName)
                {
                    if (
                        aColumnIndex!=""
                        &&
                        globalDialog->variables.at(i)->inherits("VariableExtendedListFrame")
                       )
                    {
                        aExtFrame=(VariableExtendedListFrame*)globalDialog->variables[i];
                    }
                    else
                    if (globalDialog->variables.at(i)->inherits("VariableListFrame"))
                    {
                        aFrame=(VariableListFrame*)globalDialog->variables[i];
                    }

                    break;
                }
            }
        }
        else
        {
            for (int i=0; i<mainWindow->ui->pagesTabWidget->count(); i++)
            {
                if(((PageFrame*)mainWindow->ui->pagesTabWidget->widget(i))->ui->varNameEdit->text()==aSectionName)
                {
                    PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(i);

                    for (int j=0; j<aPage->variables.length(); j++)
                    {
                        if (aPage->variables.at(j)->variableName()==aVarName)
                        {
                            if (
                                aColumnIndex!=""
                                &&
                                aPage->variables.at(j)->inherits("VariableExtendedListFrame")
                               )
                            {
                                aExtFrame=(VariableExtendedListFrame*)aPage->variables[j];
                            }
                            else
                            if (aPage->variables.at(j)->inherits("VariableListFrame"))
                            {
                                aFrame=(VariableListFrame*)aPage->variables[j];
                            }

                            break;
                        }
                    }

                    for (int j=0; j<aPage->components.length(); j++)
                    {
                        if (aPage->components.at(j)->variableName()==aVarName)
                        {
                            if (
                                aColumnIndex!=""
                                &&
                                aPage->components.at(j)->inherits("VariableExtendedListFrame")
                               )
                            {
                                aExtFrame=(VariableExtendedListFrame*)aPage->components[j];
                            }

                            break;
                        }
                    }
                }
            }
        }

        if (aExtFrame)
        {
            bool ok;
            int aColumnID=aColumnIndex.toInt(&ok)-1;

            if (ok && aColumnID<aExtFrame->typeColumns.length())
            {
                EColumnType aColumnType=aExtFrame->typeColumns.at(aColumnID).column->type();

                if (aColumnType!=ctBool && aColumnType!=ctExpression)
                {
                    QStringList aItems;

                    for (int i=0; i<aExtFrame->ui->dataTableWidget->rowCount(); i++)
                    {
                        aItems.append(aExtFrame->ui->dataTableWidget->item(i, aColumnID)->text());
                    }

                    aItems.removeDuplicates();

                    if (aColumnType==ctInteger)
                    {
                        QString aPrefix;
                        QString aPostfix;

                        if (
                            !((IntegerColumn*)aExtFrame->typeColumns.at(aColumnID).column)->mIsAutoInc
                            &&
                            !((IntegerColumn*)aExtFrame->typeColumns.at(aColumnID).column)->mSplitRows
                           )
                        {
                            aPrefix=((IntegerColumn*)aExtFrame->typeColumns.at(aColumnID).column)->mPrefix;
                            aPostfix=((IntegerColumn*)aExtFrame->typeColumns.at(aColumnID).column)->mPostfix;
                        }

                        int aDecimal=((IntegerColumn*)aExtFrame->typeColumns.at(aColumnID).column)->mDecimals;

                        QList<double> doubles;

                        for (int i=0; i<aItems.length(); i++)
                        {
                            QString aText=aItems.at(i);

                            aText.remove(aText.length()-aPostfix.length(), aPostfix.length());
                            aText.remove(0, aPrefix.length());

                            doubles.append(aText.toDouble());
                        }

                        for (int e=0; e<doubles.length()-1; e++)
                        {
                            int minIndex=e;
                            double minDouble=doubles.at(e);

                            for (int i=e+1; i<doubles.length(); i++)
                            {
                                if (doubles.at(i)<minDouble)
                                {
                                    minDouble=doubles.at(i);
                                    minIndex=i;
                                }
                            }

                            doubles.swap(e, minIndex);
                        }

                        aItems.clear();

                        for (int i=0; i<doubles.length(); i++)
                        {
                            aItems.append(aPrefix+QString::number(doubles.at(i), 'f', aDecimal)+aPostfix);
                        }
                    }
                    else
                    if (aColumnType==ctDate)
                    {
                        QList<QDate> dates;

                        for (int i=0; i<aItems.length(); i++)
                        {
                            dates.append(QDate::fromString(aItems.at(i), "dd.MM.yyyy"));
                        }

                        for (int e=0; e<dates.length()-1; e++)
                        {
                            int minIndex=e;
                            QDate minDate=dates.at(e);

                            for (int i=e+1; i<dates.length(); i++)
                            {
                                if (dates.at(i)<minDate)
                                {
                                    minDate=dates.at(i);
                                    minIndex=i;
                                }
                            }

                            dates.swap(e, minIndex);
                        }

                        aItems.clear();

                        for (int i=0; i<dates.length(); i++)
                        {
                            aItems.append(dates.at(i).toString("dd.MM.yyyy"));
                        }
                    }
                    else
                    {
                        aItems.sort();
                    }

                    editor->addItems(aItems);
                }
            }
        }
        else
        if (aFrame)
        {
            for (int i=0; i<aFrame->ui->valueComboBox->count(); i++)
            {
                editor->addItem(aFrame->ui->valueComboBox->itemText(i));
            }
        }
    }

    return editor;
}

void ListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = (QComboBox*)(editor);

    QString aValue=index.model()->data(index, Qt::EditRole).toString();

    int itemIndex=comboBox->findText(aValue);

    if (itemIndex>=0)
    {
        comboBox->setCurrentIndex(itemIndex);
    }
    else
    {
        comboBox->setEditText(aValue);
    }

    comboBox->lineEdit()->selectAll();
}

void ListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = (QComboBox*)(editor);

    model->setData(index, comboBox->currentText(), Qt::EditRole);
}

void ListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
