#include "src/other/global.h"

ListDelegate::ListDelegate(QObject *parent)
    : QItemDelegate(parent)
{
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

                    for (int i=0; i<aPage->variables.length(); i++)
                    {
                        if (aPage->variables.at(i)->variableName()==aVarName)
                        {
                            if (
                                aColumnIndex!=""
                                &&
                                aPage->variables.at(i)->inherits("VariableExtendedListFrame")
                               )
                            {
                                aExtFrame=(VariableExtendedListFrame*)aPage->variables[i];
                            }
                            else
                            if (aPage->variables.at(i)->inherits("VariableListFrame"))
                            {
                                aFrame=(VariableListFrame*)aPage->variables[i];
                            }

                            break;
                        }
                    }

                    for (int i=0; i<aPage->components.length(); i++)
                    {
                        if (aPage->components.at(i)->variableName()==aVarName)
                        {
                            if (
                                aColumnIndex!=""
                                &&
                                aPage->components.at(i)->inherits("VariableExtendedListFrame")
                               )
                            {
                                aExtFrame=(VariableExtendedListFrame*)aPage->components[i];
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

                    if (aColumnType==ctDate)
                    {
                        for (int i=0; i<aItems.length(); i++)
                        {
                            aItems[i]=QDate::fromString(aItems.at(i), "yyyy-MM-dd").toString("dd.MM.yyyy");
                        }
                    }

                    aItems.sort();

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
