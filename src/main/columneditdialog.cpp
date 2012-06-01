#include "src/other/global.h"

ColumnEditDialog::ColumnEditDialog(bool aEditMode, UnitedTable *aTableWidget, VariableExtendedListFrame *aTable, int aColumnIndex, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnEditDialog)
{
    ui->setupUi(this);

    mTableWidget=aTableWidget;
    mTable=aTable;
    mEditMode=aEditMode;
    mColumnIndex=aColumnIndex;



    ui->listLinkPagesListWidget->addItem("Global");

    for (int i=0; i<mainWindow->ui->pagesTabWidget->count(); i++)
    {
        ui->listLinkPagesListWidget->addItem(((PageFrame*)mainWindow->ui->pagesTabWidget->widget(i))->ui->varNameEdit->text());
    }

    ui->listLinkPagesListWidget->setCurrentRow(globalDialog->isVisible()? 0 : mainWindow->ui->pagesTabWidget->currentIndex()+1);



    ui->extListLinkPagesListWidget->addItem("Global");

    for (int i=0; i<mainWindow->ui->pagesTabWidget->count(); i++)
    {
        ui->extListLinkPagesListWidget->addItem(((PageFrame*)mainWindow->ui->pagesTabWidget->widget(i))->ui->varNameEdit->text());
    }

    ui->extListLinkPagesListWidget->setCurrentRow(globalDialog->isVisible()? 0 : mainWindow->ui->pagesTabWidget->currentIndex()+1);



    if (mEditMode)
    {
        startEditing();
    }
    else
    {
        ui->leftButton->setVisible(false);
        ui->rightButton->setVisible(false);
    }
}

ColumnEditDialog::~ColumnEditDialog()
{
    applyChanges();

    delete ui;
}

void ColumnEditDialog::startEditing()
{
    ui->leftButton->setEnabled(mColumnIndex>0);
    ui->rightButton->setEnabled(mColumnIndex<mTable->typeColumns.length()-1);

    STableColumn *aColumn=&mTable->typeColumns[mColumnIndex];

    ui->nameEdit->setText(aColumn->name);
    ui->visibleCheckBox->setChecked(aColumn->visible);

    switch (aColumn->column->type())
    {
        case ctInteger:
        {
            ui->typeComboBox->setCurrentIndex(0);

            ui->integerDefaultSpinBox->setValue         (((IntegerColumn*)aColumn->column)->mDefaultValue);
            ui->integerNumberSpinBox->setValue          (((IntegerColumn*)aColumn->column)->mDecimals);
            ui->integerAutoIncrementCheckBox->setChecked(((IntegerColumn*)aColumn->column)->mIsAutoInc);
            ui->integerSplitRowsCheckBox->setChecked    (((IntegerColumn*)aColumn->column)->mSplitRows);
            ui->integerPrefixEdit->setText              (((IntegerColumn*)aColumn->column)->mPrefix);
            ui->integerPostfixEdit->setText             (((IntegerColumn*)aColumn->column)->mPostfix);
        }
        break;
        case ctString:
        {
            ui->typeComboBox->setCurrentIndex(1);

            ui->stringDefaultEdit->setText(((StringColumn*)aColumn->column)->mDefaultValue);
        }
        break;
        case ctBool:
        {
            ui->typeComboBox->setCurrentIndex(2);

            ui->boolDefaultCheckBox->setChecked(((BoolColumn*)aColumn->column)->mDefaultValue);
        }
        break;
        case ctDate:
        {
            ui->typeComboBox->setCurrentIndex(3);

            ui->dateDefaultEdit->setDate(((DateColumn*)aColumn->column)->mDefaultValue);
        }
        break;
        case ctTime:
        {
            ui->typeComboBox->setCurrentIndex(4);

            ui->timeDefaultEdit->setTime(((TimeColumn*)aColumn->column)->mDefaultValue);
        }
        break;
        case ctList:
        {
            ui->typeComboBox->setCurrentIndex(5);

            ui->listDefaultComboBox->setEditText(((ListColumn*)aColumn->column)->mDefaultValue);

            QString linkName=((ListColumn*)aColumn->column)->mLinkComponent;

            int index=linkName.indexOf(".");
            QString aSectionName=linkName.left(index);
            linkName=linkName.mid(index+1);

            index=-1;

            for (int i=0; i<ui->listLinkPagesListWidget->count(); i++)
            {
                if (ui->listLinkPagesListWidget->item(i)->text()==aSectionName)
                {
                    index=i;
                    break;
                }
            }

            if (index>=0)
            {
                ui->listLinkPagesListWidget->setCurrentRow(index);

                index=-1;

                for (int i=0; i<ui->listLinkVariablesListWidget->count(); i++)
                {
                    if (ui->listLinkVariablesListWidget->item(i)->text()==linkName)
                    {
                        index=i;
                        break;
                    }
                }

                if (index>=0)
                {
                    ui->listLinkVariablesListWidget->setCurrentRow(index);
                }
            }
        }
        break;
        case ctExtendedList:
        {
            ui->typeComboBox->setCurrentIndex(6);

            ui->extendedListComboBox->setEditText(((ExtendedListColumn*)aColumn->column)->mDefaultValue);

            QString linkName=((ListColumn*)aColumn->column)->mLinkComponent;

            int index=linkName.indexOf(".");
            QString aSectionName=linkName.left(index);
            linkName=linkName.mid(index+1);

            index=linkName.indexOf("[");
            QString aVarName=linkName.left(index);
            linkName=linkName.mid(index+1);
            linkName.remove(linkName.length()-1, 1);

            index=-1;

            for (int i=0; i<ui->extListLinkPagesListWidget->count(); i++)
            {
                if (ui->extListLinkPagesListWidget->item(i)->text()==aSectionName)
                {
                    index=i;
                    break;
                }
            }

            if (index>=0)
            {
                ui->extListLinkPagesListWidget->setCurrentRow(index);

                index=-1;

                for (int i=0; i<ui->extListLinkVariablesListWidget->count(); i++)
                {
                    if (ui->extListLinkVariablesListWidget->item(i)->text()==aVarName)
                    {
                        index=i;
                        break;
                    }
                }

                if (index>=0)
                {
                    ui->extListLinkVariablesListWidget->setCurrentRow(index);

                    int aColumnIndex=linkName.toInt()-1;

                    if (aColumnIndex<ui->extListLinkColumnsListWidget->count())
                    {
                        ui->extListLinkColumnsListWidget->setCurrentRow(aColumnIndex);
                    }
                }
            }
        }
        break;
        case ctExpression:
        {
            ui->typeComboBox->setCurrentIndex(7);

            ui->expressionModifyCheckBox->setChecked(((ExpressionColumn*)aColumn->column)->mAllowModify);
            ui->expressionEdit->setText(((ExpressionColumn*)aColumn->column)->mDefaultValue);
        }
        break;
        default:
        {
            throw "Unknown column type";
        }
        break;
    }

    ui->leftMarginSpinBox->setValue(aColumn->leftMargin);
    ui->rightMarginSpinBox->setValue(aColumn->rightMargin);
    ui->topMarginSpinBox->setValue(aColumn->topMargin);
    ui->bottomMarginSpinBox->setValue(aColumn->bottomMargin);

    while (ui->conditionsVerticalLayout->count()>0)
    {
        QWidget *aWidget=ui->conditionsVerticalLayout->itemAt(0)->widget();
        ui->conditionsVerticalLayout->removeWidget(aWidget);
        delete aWidget;
    }

    for (int i=0; i<aColumn->conditions.length(); i++)
    {
        on_addToolButton_clicked();

        QTableWidgetItem *aItem=((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(i)->widget()))->ui->exampleTableWidget->item(0, 0);

        QFont aFont;
        aFont.fromString(aColumn->conditions.at(i).fontString);
        aItem->setFont(aFont);

        aItem->setTextAlignment(aColumn->conditions.at(i).alignment);
        aItem->setBackground(QBrush(QColor(aColumn->conditions.at(i).backgroundColorR, aColumn->conditions.at(i).backgroundColorG, aColumn->conditions.at(i).backgroundColorB)));
        aItem->setTextColor(QColor(aColumn->conditions.at(i).textColorR, aColumn->conditions.at(i).textColorG, aColumn->conditions.at(i).textColorB));

        ((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(i)->widget()))->ui->conditionEdit->setText(aColumn->conditions.at(i).condition);
        ((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(i)->widget()))->ui->warningCheckBox->setChecked(aColumn->conditions.at(i).needWarning);
    }
}

void ColumnEditDialog::applyChanges()
{
    if (mEditMode)
    {
        STableColumn *aColumn=&mTable->typeColumns[mColumnIndex];

        aColumn->name=ui->nameEdit->text();
        aColumn->visible=ui->visibleCheckBox->isChecked();

        ColumnType *aOldTypeColumn=aColumn->column;

        switch (ui->typeComboBox->currentIndex())
        {
            case 0:
            {
                IntegerColumn *aTypeColumn=new IntegerColumn();

                aTypeColumn->mDefaultValue=ui->integerDefaultSpinBox->value();
                aTypeColumn->mDecimals=ui->integerNumberSpinBox->value();
                aTypeColumn->mIsAutoInc=ui->integerAutoIncrementCheckBox->isChecked();
                aTypeColumn->mSplitRows=ui->integerSplitRowsCheckBox->isChecked();
                aTypeColumn->mPrefix=ui->integerPrefixEdit->text();
                aTypeColumn->mPostfix=ui->integerPostfixEdit->text();

                aColumn->column=aTypeColumn;
            }
            break;
            case 1:
            {
                StringColumn *aTypeColumn=new StringColumn();

                aTypeColumn->mDefaultValue=ui->stringDefaultEdit->text();

                aColumn->column=aTypeColumn;
            }
            break;
            case 2:
            {
                BoolColumn *aTypeColumn=new BoolColumn();

                aTypeColumn->mDefaultValue=ui->boolDefaultCheckBox->isChecked();

                aColumn->column=aTypeColumn;
            }
            break;
            case 3:
            {
                DateColumn *aTypeColumn=new DateColumn();

                aTypeColumn->mDefaultValue=ui->dateDefaultEdit->date();

                aColumn->column=aTypeColumn;
            }
            break;
            case 4:
            {
                TimeColumn *aTypeColumn=new TimeColumn();

                aTypeColumn->mDefaultValue=ui->timeDefaultEdit->time();

                aColumn->column=aTypeColumn;
            }
            break;
            case 5:
            {
                if (ui->listLinkVariablesListWidget->currentRow()<0)
                {
                    QMessageBox::information(this, protocolCreatorVersion, "Укажите список для связки");
                    return;
                }

                ListColumn *aTypeColumn=new ListColumn();

                aTypeColumn->mDefaultValue=ui->listDefaultComboBox->currentText();
                aTypeColumn->mLinkComponent=ui->listLinkPagesListWidget->currentItem()->text()+"."+ui->listLinkVariablesListWidget->currentItem()->text();

                aColumn->column=aTypeColumn;
            }
            break;
            case 6:
            {
                if (ui->extListLinkColumnsListWidget->currentRow()<0)
                {
                    QMessageBox::information(this, protocolCreatorVersion, "Укажите столбец из расширенного списка для связки");
                    return;
                }

                ExtendedListColumn *aTypeColumn=new ExtendedListColumn();

                aTypeColumn->mDefaultValue=ui->extendedListComboBox->currentText();
                aTypeColumn->mLinkComponent=ui->extListLinkPagesListWidget->currentItem()->text()+"."+ui->extListLinkVariablesListWidget->currentItem()->text()+"["+QString::number(ui->extListLinkColumnsListWidget->currentRow()+1)+"]";

                aColumn->column=aTypeColumn;
            }
            break;
            case 7:
            {
                ExpressionColumn *aTypeColumn=new ExpressionColumn();

                aTypeColumn->mAllowModify=ui->expressionModifyCheckBox->isChecked();
                aTypeColumn->mDefaultValue=ui->expressionEdit->text();

                aColumn->column=aTypeColumn;
            }
            break;
            default:
            {
                throw "Unknown column type";
            }
            break;
        }

        aColumn->leftMargin=ui->leftMarginSpinBox->value();
        aColumn->rightMargin=ui->rightMarginSpinBox->value();
        aColumn->topMargin=ui->topMarginSpinBox->value();
        aColumn->bottomMargin=ui->bottomMarginSpinBox->value();

        aColumn->conditions.clear();

        for (int i=0; i<ui->conditionsVerticalLayout->count(); i++)
        {
            QTableWidgetItem *aItem=((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(i)->widget()))->ui->exampleTableWidget->item(0, 0);

            SConditionFormat aFormat;

            aFormat.fontString=aItem->font().toString();
            aFormat.alignment=aItem->textAlignment();
            aFormat.backgroundColorR=aItem->background().color().red();
            aFormat.backgroundColorG=aItem->background().color().green();
            aFormat.backgroundColorB=aItem->background().color().blue();
            aFormat.textColorR=aItem->textColor().red();
            aFormat.textColorG=aItem->textColor().green();
            aFormat.textColorB=aItem->textColor().blue();
            aFormat.condition=((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(i)->widget()))->ui->conditionEdit->text();
            aFormat.needWarning=((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(i)->widget()))->ui->warningCheckBox->isChecked();

            aColumn->conditions.append(aFormat);
        }



        mTableWidget->horizontalHeaderItem(mColumnIndex)->setText(aColumn->name);
        mTableWidget->horizontalHeaderItem(mColumnIndex)->setTextColor(aColumn->visible? QColor(0, 0, 0) : QColor(128, 128, 128));

        QFont aFont=mTableWidget->horizontalHeaderItem(mColumnIndex)->font();
        aFont.setUnderline(aColumn->conditions.length()>0);
        mTableWidget->horizontalHeaderItem(mColumnIndex)->setFont(aFont);

        mTableWidget->item(0, mColumnIndex)->setText(aColumn->column->typeDescription());



        mTable->ui->dataTableWidget->horizontalHeaderItem(mColumnIndex)->setText(aColumn->name);
        mTable->ui->dataTableWidget->horizontalHeaderItem(mColumnIndex)->setTextColor(aColumn->visible? QColor(0, 0, 0) : QColor(128, 128, 128));

        aFont=mTable->ui->dataTableWidget->horizontalHeaderItem(mColumnIndex)->font();
        aFont.setUnderline(aColumn->conditions.length()>0);
        mTable->ui->dataTableWidget->horizontalHeaderItem(mColumnIndex)->setFont(aFont);

        delete mTable->ui->dataTableWidget->itemDelegateForColumn(mColumnIndex);

        switch (ui->typeComboBox->currentIndex())
        {
            case 0:
            {
                DoubleDelegate *delegate=new DoubleDelegate(mTable);

                delegate->mDecimals=ui->integerNumberSpinBox->value();

                if (
                    !ui->integerAutoIncrementCheckBox->isChecked()
                    &&
                    !ui->integerSplitRowsCheckBox->isChecked()
                   )
                {
                    delegate->mPrefix=ui->integerPrefixEdit->text();
                    delegate->mPostfix=ui->integerPostfixEdit->text();
                }

                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, delegate);
            }
            break;
            case 3:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new DateDelegate(mTable));
            }
            break;
            case 4:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new TimeDelegate(mTable));
            }
            break;
            case 5:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new ListDelegate(
                                                                                                     ui->listLinkPagesListWidget->currentItem()->text()+"."+ui->listLinkVariablesListWidget->currentItem()->text(),
                                                                                                     mTable
                                                                                                    ));
            }
            break;
            case 6:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new ListDelegate(
                                                                                                     ui->extListLinkPagesListWidget->currentItem()->text()+"."+ui->extListLinkVariablesListWidget->currentItem()->text()+"["+QString::number(ui->extListLinkColumnsListWidget->currentRow()+1)+"]",
                                                                                                     mTable
                                                                                                    ));
            }
            break;
            default:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new StringDelegate(mTable));
            }
            break;
        }

        EColumnType aOldColumnType=aOldTypeColumn->type();
        EColumnType aNewColumnType=aColumn->column->type();

        if (aOldColumnType==ctBool)
        {
            for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
            {
                if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
                {
                    continue;
                }

                if (mTable->ui->dataTableWidget->item(i, mColumnIndex)->checkState()==Qt::Checked)
                {
                    mTable->ui->dataTableWidget->item(i, mColumnIndex)->setText("1");
                }
                else
                {
                    mTable->ui->dataTableWidget->item(i, mColumnIndex)->setText("0");
                }

                QMap<int, QVariant> dataRoles=mTable->ui->dataTableWidget->model()->itemData(mTable->ui->dataTableWidget->indexFromItem(mTable->ui->dataTableWidget->item(i, mColumnIndex)));

                delete mTable->ui->dataTableWidget->takeItem(i, mColumnIndex);

                QTableWidgetItem *aItem=new QTableWidgetItem();

                for (QMap<int, QVariant>::ConstIterator j=dataRoles.constBegin(); j!=dataRoles.constEnd(); j++)
                {
                    if (j.key()!=Qt::CheckStateRole)
                    {
                        aItem->setData(j.key(), j.value());
                    }
                }

                mTable->ui->dataTableWidget->setItem(i, mColumnIndex, aItem);
            }

            aOldColumnType=ctString;
        }

        if (aNewColumnType==ctInteger)
        {
            if (ui->integerAutoIncrementCheckBox->isChecked())
            {
                int id=1;

                for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
                {
                    if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
                    {
                        continue;
                    }

                    mTable->ui->dataTableWidget->item(i, mColumnIndex)->setText(QString::number(id));

                    id++;
                }
            }
            else
            {
                int removeBefore=0;
                int removeAfter=0;

                if (
                    aOldColumnType==ctInteger
                    &&
                    !((IntegerColumn*)aOldTypeColumn)->mIsAutoInc
                    &&
                    !((IntegerColumn*)aOldTypeColumn)->mSplitRows
                   )
                {
                    removeBefore=((IntegerColumn*)aOldTypeColumn)->mPrefix.length();
                    removeAfter=((IntegerColumn*)aOldTypeColumn)->mPostfix.length();
                }

                for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
                {
                    if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
                    {
                        continue;
                    }

                    QString aText=mTable->ui->dataTableWidget->item(i, mColumnIndex)->text();

                    aText.remove(aText.length()-removeAfter, removeAfter);
                    aText.remove(0, removeBefore);

                    bool ok;
                    double aValue=aText.toDouble(&ok);

                    if (ok)
                    {
                        aText=QString::number(aValue, 'f', ui->integerNumberSpinBox->value());
                    }
                    else
                    {
                        aText=QString::number(ui->integerDefaultSpinBox->value(), 'f', ui->integerNumberSpinBox->value());
                    }

                    if (ui->integerSplitRowsCheckBox->isChecked())
                    {
                        mTable->ui->dataTableWidget->item(i, mColumnIndex)->setText(aText);
                    }
                    else
                    {
                        mTable->ui->dataTableWidget->item(i, mColumnIndex)->setText(ui->integerPrefixEdit->text()+aText+ui->integerPostfixEdit->text());
                    }
                }
            }
        }
        else
        if (aNewColumnType==ctString || aNewColumnType==ctList || aNewColumnType==ctExtendedList)
        {
            // Nothing
        }
        else
        if (aNewColumnType==ctBool)
        {
            for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
            {
                if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
                {
                    continue;
                }

                bool ok;
                double aValue=mTable->ui->dataTableWidget->item(i, mColumnIndex)->text().toDouble(&ok);

                if (ok)
                {
                    if (aValue==0)
                    {
                        mTable->ui->dataTableWidget->item(i, mColumnIndex)->setCheckState(Qt::Unchecked);
                    }
                    else
                    if (
                        aOldColumnType!=ctInteger
                        ||
                        !((IntegerColumn*)aOldTypeColumn)->mIsAutoInc
                       )
                    {
                        mTable->ui->dataTableWidget->item(i, mColumnIndex)->setCheckState(Qt::Checked);
                    }
                    else
                    {
                        ok=false;
                    }
                }

                if (!ok)
                {
                    if (ui->boolDefaultCheckBox->isChecked())
                    {
                        mTable->ui->dataTableWidget->item(i, mColumnIndex)->setCheckState(Qt::Checked);
                    }
                    else
                    {
                        mTable->ui->dataTableWidget->item(i, mColumnIndex)->setCheckState(Qt::Unchecked);
                    }
                }

                mTable->ui->dataTableWidget->item(i, mColumnIndex)->setText("");
            }
        }
        else
        if (aNewColumnType==ctDate)
        {
            for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
            {
                if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
                {
                    continue;
                }

                QDate aDate=QDate::fromString(mTable->ui->dataTableWidget->item(i, mColumnIndex)->text(), "dd.MM.yyyy");

                if (!aDate.isValid())
                {
                    mTable->ui->dataTableWidget->item(i, mColumnIndex)->setText(ui->dateDefaultEdit->date().toString("dd.MM.yyyy"));
                }
            }
        }
        else
        if (aNewColumnType==ctTime)
        {
            for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
            {
                if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
                {
                    continue;
                }

                QTime aTime=QTime::fromString(mTable->ui->dataTableWidget->item(i, mColumnIndex)->text(), "h:mm:ss");

                if (!aTime.isValid())
                {
                    mTable->ui->dataTableWidget->item(i, mColumnIndex)->setText(ui->timeDefaultEdit->time().toString("hh:mm:ss"));
                }
            }
        }
        else
        if (aNewColumnType==ctExpression)
        {
            if (!ui->expressionModifyCheckBox->isChecked())
            {
                for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
                {
                    if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
                    {
                        continue;
                    }

                    mTable->ui->dataTableWidget->item(i, mColumnIndex)->setText(ui->expressionEdit->text());
                }
            }
        }
        else
        {
            throw "Unknown column type";
        }

        delete aOldTypeColumn;
    }
    else
    {
        STableColumn aColumn;

        aColumn.name=ui->nameEdit->text();
        aColumn.visible=ui->visibleCheckBox->isChecked();

        switch (ui->typeComboBox->currentIndex())
        {
            case 0:
            {
                IntegerColumn *aTypeColumn=new IntegerColumn();

                aTypeColumn->mDefaultValue=ui->integerDefaultSpinBox->value();
                aTypeColumn->mDecimals=ui->integerNumberSpinBox->value();
                aTypeColumn->mIsAutoInc=ui->integerAutoIncrementCheckBox->isChecked();
                aTypeColumn->mSplitRows=ui->integerSplitRowsCheckBox->isChecked();
                aTypeColumn->mPrefix=ui->integerPrefixEdit->text();
                aTypeColumn->mPostfix=ui->integerPostfixEdit->text();

                aColumn.column=aTypeColumn;
            }
            break;
            case 1:
            {
                StringColumn *aTypeColumn=new StringColumn();

                aTypeColumn->mDefaultValue=ui->stringDefaultEdit->text();

                aColumn.column=aTypeColumn;
            }
            break;
            case 2:
            {
                BoolColumn *aTypeColumn=new BoolColumn();

                aTypeColumn->mDefaultValue=ui->boolDefaultCheckBox->isChecked();

                aColumn.column=aTypeColumn;
            }
            break;
            case 3:
            {
                DateColumn *aTypeColumn=new DateColumn();

                aTypeColumn->mDefaultValue=ui->dateDefaultEdit->date();

                aColumn.column=aTypeColumn;
            }
            break;
            case 4:
            {
                TimeColumn *aTypeColumn=new TimeColumn();

                aTypeColumn->mDefaultValue=ui->timeDefaultEdit->time();

                aColumn.column=aTypeColumn;
            }
            break;
            case 5:
            {
                if (ui->listLinkVariablesListWidget->currentRow()<0)
                {
                    QMessageBox::information(this, protocolCreatorVersion, "Укажите список для связки");
                    return;
                }

                ListColumn *aTypeColumn=new ListColumn();

                aTypeColumn->mDefaultValue=ui->listDefaultComboBox->currentText();
                aTypeColumn->mLinkComponent=ui->listLinkPagesListWidget->currentItem()->text()+"."+ui->listLinkVariablesListWidget->currentItem()->text();

                aColumn.column=aTypeColumn;
            }
            break;
            case 6:
            {
                if (ui->extListLinkColumnsListWidget->currentRow()<0)
                {
                    QMessageBox::information(this, protocolCreatorVersion, "Укажите столбец из расширенного списка для связки");
                    return;
                }

                ExtendedListColumn *aTypeColumn=new ExtendedListColumn();

                aTypeColumn->mDefaultValue=ui->extendedListComboBox->currentText();
                aTypeColumn->mLinkComponent=ui->extListLinkPagesListWidget->currentItem()->text()+"."+ui->extListLinkVariablesListWidget->currentItem()->text()+"["+QString::number(ui->extListLinkColumnsListWidget->currentRow()+1)+"]";

                aColumn.column=aTypeColumn;
            }
            break;
            case 7:
            {
                ExpressionColumn *aTypeColumn=new ExpressionColumn();

                aTypeColumn->mAllowModify=ui->expressionModifyCheckBox->isChecked();
                aTypeColumn->mDefaultValue=ui->expressionEdit->text();

                aColumn.column=aTypeColumn;
            }
            break;
            default:
            {
                throw "Unknown column type";
            }
            break;
        }

        aColumn.leftMargin=ui->leftMarginSpinBox->value();
        aColumn.rightMargin=ui->rightMarginSpinBox->value();
        aColumn.topMargin=ui->topMarginSpinBox->value();
        aColumn.bottomMargin=ui->bottomMarginSpinBox->value();
        aColumn.fontString="";
        aColumn.alignment=Qt::AlignTop | Qt::AlignLeft;
        aColumn.backgroundColorR=255;
        aColumn.backgroundColorG=255;
        aColumn.backgroundColorB=255;
        aColumn.textColorR=0;
        aColumn.textColorG=0;
        aColumn.textColorB=0;

        for (int i=0; i<ui->conditionsVerticalLayout->count(); i++)
        {
            QTableWidgetItem *aItem=((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(i)->widget()))->ui->exampleTableWidget->item(0, 0);

            SConditionFormat aFormat;

            aFormat.fontString=aItem->font().toString();
            aFormat.alignment=aItem->textAlignment();
            aFormat.backgroundColorR=aItem->background().color().red();
            aFormat.backgroundColorG=aItem->background().color().green();
            aFormat.backgroundColorB=aItem->background().color().blue();
            aFormat.textColorR=aItem->textColor().red();
            aFormat.textColorG=aItem->textColor().green();
            aFormat.textColorB=aItem->textColor().blue();
            aFormat.condition=((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(i)->widget()))->ui->conditionEdit->text();
            aFormat.needWarning=((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(i)->widget()))->ui->warningCheckBox->isChecked();

            aColumn.conditions.append(aFormat);
        }

        mTable->typeColumns.insert(mColumnIndex, aColumn);

        mTableWidget->insertColumn(mColumnIndex);

        QTableWidgetItem *aItem;

        aItem=new QTableWidgetItem(aColumn.name);
        aItem->setTextColor(aColumn.visible? QColor(0, 0, 0) : QColor(128, 128, 128));

        if (aColumn.conditions.length()>0)
        {
            QFont aFont=aItem->font();
            aFont.setUnderline(true);
            aItem->setFont(aFont);
        }

        mTableWidget->setHorizontalHeaderItem(mColumnIndex, aItem);



        QBrush aBrush(QColor(255, 255, 255));

        aItem=new QTableWidgetItem(aColumn.column->typeDescription());
        aItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
        aItem->setFont(QFont("Times New Roman", 12));
        aItem->setBackground(aBrush);

        mTableWidget->setItem(0, mColumnIndex, aItem);

        aItem=new QTableWidgetItem("Промежуточная строка");
        aItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
        aItem->setFont(QFont("Times New Roman", 12));
        aItem->setBackground(aBrush);

        mTableWidget->setItem(1, mColumnIndex, aItem);

        if (mTableWidget->columnCount()>1)
        {
            int firstCell=0;

            if (mColumnIndex==0)
            {
                firstCell++;
            }

            mTableWidget->item(1, mColumnIndex)->setFont(mTableWidget->item(1, firstCell)->font());
            mTableWidget->item(1, mColumnIndex)->setBackground(mTableWidget->item(1, firstCell)->background());
            mTableWidget->item(1, mColumnIndex)->setTextColor(mTableWidget->item(1, firstCell)->textColor());
            mTableWidget->item(1, mColumnIndex)->setTextAlignment(mTableWidget->item(1, firstCell)->textAlignment());

            mTableWidget->separate(1, firstCell);

            mTableWidget->unite(0, 1, mTableWidget->columnCount()-1, 1);
        }

        if (mTable->ui->dataTableWidget->columnCount()>1)
        {
            for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
            {
                if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
                {
                    mTable->ui->dataTableWidget->setSpan(i, 0, 1, 1);
                }
            }
        }

        mTable->ui->dataTableWidget->insertColumn(mColumnIndex);

        aItem=new QTableWidgetItem(aColumn.name);
        aItem->setTextColor(aColumn.visible? QColor(0, 0, 0) : QColor(128, 128, 128));

        if (aColumn.conditions.length()>0)
        {
            QFont aFont=aItem->font();
            aFont.setUnderline(true);
            aItem->setFont(aFont);
        }

        mTable->ui->dataTableWidget->setHorizontalHeaderItem(mColumnIndex, aItem);

        for (int i=mTable->ui->dataTableWidget->columnCount()-1; i>mColumnIndex; i--)
        {
            mTable->ui->dataTableWidget->setItemDelegateForColumn(i, mTable->ui->dataTableWidget->itemDelegateForColumn(i-1));
        }

        switch (ui->typeComboBox->currentIndex())
        {
            case 0:
            {
                DoubleDelegate *delegate=new DoubleDelegate(mTable);

                delegate->mDecimals=ui->integerNumberSpinBox->value();

                if (
                    !ui->integerAutoIncrementCheckBox->isChecked()
                    &&
                    !ui->integerSplitRowsCheckBox->isChecked()
                   )
                {
                    delegate->mPrefix=ui->integerPrefixEdit->text();
                    delegate->mPostfix=ui->integerPostfixEdit->text();
                }

                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, delegate);
            }
            break;
            case 3:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new DateDelegate(mTable));
            }
            break;
            case 4:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new TimeDelegate(mTable));
            }
            break;
            case 5:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new ListDelegate(
                                                                                                     ui->listLinkPagesListWidget->currentItem()->text()+"."+ui->listLinkVariablesListWidget->currentItem()->text(),
                                                                                                     mTable
                                                                                                    ));
            }
            break;
            case 6:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new ListDelegate(
                                                                                                     ui->extListLinkPagesListWidget->currentItem()->text()+"."+ui->extListLinkVariablesListWidget->currentItem()->text()+"["+QString::number(ui->extListLinkColumnsListWidget->currentRow()+1)+"]",
                                                                                                     mTable
                                                                                                    ));
            }
            break;
            default:
            {
                mTable->ui->dataTableWidget->setItemDelegateForColumn(mColumnIndex, new StringDelegate(mTable));
            }
            break;
        }

        for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
        {
            QTableWidgetItem *aItem=new QTableWidgetItem();

            aItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
            aItem->setFont(QFont("Times New Roman", 12));
            aItem->setBackground(QBrush(QColor(255, 255, 255)));

            if (mTable->ui->dataTableWidget->itemDelegateForRow(i)==0)
            {
                switch (aColumn.column->type())
                {
                    case ctInteger:
                    {
                        if (((IntegerColumn*)aColumn.column)->mIsAutoInc)
                        {
                            int id=1;

                            for (int j=0; j<i; j++)
                            {
                                if (mTable->ui->dataTableWidget->itemDelegateForRow(j)==0)
                                {
                                    id++;
                                }
                            }

                            aItem->setText(QString::number(id));
                        }
                        else
                        if (((IntegerColumn*)aColumn.column)->mSplitRows)
                        {
                            aItem->setText(QString::number(((IntegerColumn*)aColumn.column)->mDefaultValue, 'f', ((IntegerColumn*)aColumn.column)->mDecimals));
                        }
                        else
                        {
                            aItem->setText(((IntegerColumn*)aColumn.column)->mPrefix+QString::number(((IntegerColumn*)aColumn.column)->mDefaultValue, 'f', ((IntegerColumn*)aColumn.column)->mDecimals)+((IntegerColumn*)aColumn.column)->mPostfix);
                        }
                    }
                    break;
                    case ctString:
                    {
                        aItem->setText((((StringColumn*)aColumn.column)->mDefaultValue));
                    }
                    break;
                    case ctBool:
                    {
                        if (((BoolColumn*)aColumn.column)->mDefaultValue)
                        {
                            aItem->setCheckState(Qt::Checked);
                        }
                        else
                        {
                            aItem->setCheckState(Qt::Unchecked);
                        }
                    }
                    break;
                    case ctDate:
                    {
                        aItem->setText(((DateColumn*)aColumn.column)->mDefaultValue.toString("dd.MM.yyyy"));
                    }
                    break;
                    case ctTime:
                    {
                        aItem->setText(((TimeColumn*)aColumn.column)->mDefaultValue.toString("hh:mm:ss"));
                    }
                    break;
                    case ctList:
                    {
                        aItem->setText((((ListColumn*)aColumn.column)->mDefaultValue));
                    }
                    break;
                    case ctExtendedList:
                    {
                        aItem->setText((((ExtendedListColumn*)aColumn.column)->mDefaultValue));
                    }
                    break;
                    case ctExpression:
                    {
                        aItem->setText((((ExpressionColumn*)aColumn.column)->mDefaultValue));
                    }
                    break;
                    default:
                    break;
                }
            }

            mTable->ui->dataTableWidget->setItem(i, mColumnIndex, aItem);
        }

        if (mTable->ui->dataTableWidget->columnCount()>1)
        {
            for (int i=0; i<mTable->ui->dataTableWidget->rowCount(); i++)
            {
                if (mTable->ui->dataTableWidget->itemDelegateForRow(i))
                {
                    mTable->ui->dataTableWidget->setSpan(i, 0, 1, mTable->ui->dataTableWidget->columnCount());
                }
            }
        }
    }
}

void ColumnEditDialog::on_typeComboBox_currentIndexChanged(int index)
{
    ui->typeStackedWidget->setCurrentIndex(index);
}

void ColumnEditDialog::on_integerNumberSpinBox_valueChanged(int value)
{
    ui->integerDefaultSpinBox->setDecimals(value);
}

void ColumnEditDialog::on_integerAutoIncrementCheckBox_toggled(bool checked)
{
    ui->autoIncWidget->setVisible(!checked);
    ui->integerSplitRowsCheckBox->setEnabled(!checked);
}

void ColumnEditDialog::on_integerSplitRowsCheckBox_toggled(bool checked)
{
    if (checked)
    {
        ui->integerNumberSpinBox->setValue(0);
    }

    ui->integerAutoIncrementCheckBox->setEnabled(!checked);
    ui->integerNumberLabel->setVisible(!checked);
    ui->integerNumberSpinBox->setVisible(!checked);
}

void ColumnEditDialog::on_listLinkPagesListWidget_currentRowChanged(int currentRow)
{
    QString currentDefault=ui->listDefaultComboBox->currentText();

    ui->listDefaultComboBox->clear();
    ui->listLinkVariablesListWidget->clear();

    if (currentRow>=0)
    {
        if (currentRow==0)
        {
            for (int i=0; i<globalDialog->variables.length(); i++)
            {
                if (globalDialog->variables.at(i)->inherits("VariableListFrame"))
                {
                    ui->listLinkVariablesListWidget->addItem(globalDialog->variables.at(i)->variableName());
                }
            }
        }
        else
        {
            PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(currentRow-1);

            for (int i=0; i<aPage->variables.length(); i++)
            {
                if (aPage->variables.at(i)->inherits("VariableListFrame"))
                {
                    ui->listLinkVariablesListWidget->addItem(aPage->variables.at(i)->variableName());
                }
            }
        }
    }

    ui->listDefaultComboBox->setEditText(currentDefault);
}

void ColumnEditDialog::on_listLinkVariablesListWidget_currentRowChanged(int currentRow)
{
    QString currentDefault=ui->listDefaultComboBox->currentText();

    ui->listDefaultComboBox->clear();

    if (currentRow>=0)
    {
        QString varName=ui->listLinkVariablesListWidget->item(currentRow)->text();
        VariableListFrame *aFrame=0;

        if (ui->listLinkPagesListWidget->currentRow()==0)
        {
            for (int i=0; i<globalDialog->variables.length(); i++)
            {
                if (
                    globalDialog->variables.at(i)->variableName()==varName
                    &&
                    globalDialog->variables.at(i)->inherits("VariableListFrame")
                   )
                {
                    aFrame=(VariableListFrame*)globalDialog->variables[i];
                    break;
                }
            }
        }
        else
        {
            PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(ui->listLinkPagesListWidget->currentRow()-1);

            for (int i=0; i<aPage->variables.length(); i++)
            {
                if (
                    aPage->variables.at(i)->variableName()==varName
                    &&
                    aPage->variables.at(i)->inherits("VariableListFrame")
                   )
                {
                    aFrame=(VariableListFrame*)aPage->variables[i];
                    break;
                }
            }
        }

        if (aFrame)
        {
            for (int i=0; i<aFrame->ui->valueComboBox->count(); i++)
            {
                ui->listDefaultComboBox->addItem(aFrame->ui->valueComboBox->itemText(i));
            }
        }
    }

    int index=ui->listDefaultComboBox->findText(currentDefault);

    if (index>=0)
    {
        ui->listDefaultComboBox->setCurrentIndex(index);
    }
    else
    {
        ui->listDefaultComboBox->setEditText(currentDefault);
    }
}

void ColumnEditDialog::on_extListLinkPagesListWidget_currentRowChanged(int currentRow)
{
    QString currentDefault=ui->extendedListComboBox->currentText();

    ui->extendedListComboBox->clear();
    ui->extListLinkVariablesListWidget->clear();
    ui->extListLinkColumnsListWidget->clear();

    if (currentRow>=0)
    {
        if (currentRow==0)
        {
            for (int i=0; i<globalDialog->variables.length(); i++)
            {
                if (globalDialog->variables.at(i)->inherits("VariableExtendedListFrame"))
                {
                    ui->extListLinkVariablesListWidget->addItem(globalDialog->variables.at(i)->variableName());
                }
            }
        }
        else
        {
            PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(currentRow-1);

            for (int i=0; i<aPage->variables.length(); i++)
            {
                if (aPage->variables.at(i)->inherits("VariableExtendedListFrame"))
                {
                    ui->extListLinkVariablesListWidget->addItem(aPage->variables.at(i)->variableName());
                }
            }

            for (int i=0; i<aPage->components.length(); i++)
            {
                if (aPage->components.at(i)->inherits("VariableExtendedListFrame"))
                {
                    ui->extListLinkVariablesListWidget->addItem(aPage->components.at(i)->variableName());
                }
            }
        }
    }

    ui->extendedListComboBox->setEditText(currentDefault);
}

void ColumnEditDialog::on_extListLinkVariablesListWidget_currentRowChanged(int currentRow)
{
    QString currentDefault=ui->extendedListComboBox->currentText();

    ui->extendedListComboBox->clear();
    ui->extListLinkColumnsListWidget->clear();

    if (currentRow>=0)
    {
        QString varName=ui->extListLinkVariablesListWidget->item(currentRow)->text();
        VariableExtendedListFrame *aFrame=0;

        if (ui->extListLinkPagesListWidget->currentRow()==0)
        {
            for (int i=0; i<globalDialog->variables.length(); i++)
            {
                if (
                    globalDialog->variables.at(i)->variableName()==varName
                    &&
                    globalDialog->variables.at(i)->inherits("VariableExtendedListFrame")
                   )
                {
                    aFrame=(VariableExtendedListFrame*)globalDialog->variables[i];
                    break;
                }
            }
        }
        else
        {
            PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(ui->extListLinkPagesListWidget->currentRow()-1);

            for (int i=0; i<aPage->variables.length(); i++)
            {
                if (
                    aPage->variables.at(i)->variableName()==varName
                    &&
                    aPage->variables.at(i)->inherits("VariableExtendedListFrame")
                   )
                {
                    aFrame=(VariableExtendedListFrame*)aPage->variables[i];
                    break;
                }
            }

            if (aFrame==0)
            {
                for (int i=0; i<aPage->components.length(); i++)
                {
                    if (
                        aPage->components.at(i)->variableName()==varName
                        &&
                        aPage->components.at(i)->inherits("VariableExtendedListFrame")
                       )
                    {
                        aFrame=(VariableExtendedListFrame*)aPage->components[i];
                        break;
                    }
                }
            }
        }

        if (aFrame)
        {
            for (int i=0; i<aFrame->typeColumns.length(); i++)
            {
                ui->extListLinkColumnsListWidget->addItem("Столбец_"+QString::number(i+1)+" "+aFrame->typeColumns.at(i).name);
            }
        }
    }

    ui->extendedListComboBox->setEditText(currentDefault);
}

void ColumnEditDialog::on_extListLinkColumnsListWidget_currentRowChanged(int currentRow)
{
    QString currentDefault=ui->extendedListComboBox->currentText();

    ui->extendedListComboBox->clear();

    if (currentRow>=0)
    {
        QString varName=ui->extListLinkVariablesListWidget->currentItem()->text();
        VariableExtendedListFrame *aFrame=0;

        if (ui->extListLinkPagesListWidget->currentRow()==0)
        {
            for (int i=0; i<globalDialog->variables.length(); i++)
            {
                if (
                    globalDialog->variables.at(i)->variableName()==varName
                    &&
                    globalDialog->variables.at(i)->inherits("VariableExtendedListFrame")
                   )
                {
                    aFrame=(VariableExtendedListFrame*)globalDialog->variables[i];
                    break;
                }
            }
        }
        else
        {
            PageFrame *aPage=(PageFrame*)mainWindow->ui->pagesTabWidget->widget(ui->extListLinkPagesListWidget->currentRow()-1);

            for (int i=0; i<aPage->variables.length(); i++)
            {
                if (
                    aPage->variables.at(i)->variableName()==varName
                    &&
                    aPage->variables.at(i)->inherits("VariableExtendedListFrame")
                   )
                {
                    aFrame=(VariableExtendedListFrame*)aPage->variables[i];
                    break;
                }
            }

            if (aFrame==0)
            {
                for (int i=0; i<aPage->components.length(); i++)
                {
                    if (
                        aPage->components.at(i)->variableName()==varName
                        &&
                        aPage->components.at(i)->inherits("VariableExtendedListFrame")
                       )
                    {
                        aFrame=(VariableExtendedListFrame*)aPage->components[i];
                        break;
                    }
                }
            }
        }

        if (aFrame)
        {
            EColumnType aColumnType=aFrame->typeColumns.at(currentRow).column->type();

            if (aColumnType!=ctBool && aColumnType!=ctExpression)
            {
                QStringList aItems;

                for (int i=0; i<aFrame->ui->dataTableWidget->rowCount(); i++)
                {
                    aItems.append(aFrame->ui->dataTableWidget->item(i, currentRow)->text());
                }

                aItems.removeDuplicates();

                if (aColumnType==ctInteger)
                {
                    int aDecimal=((IntegerColumn*)aFrame->typeColumns.at(currentRow).column)->mDecimals;

                    QString aPrefix;
                    QString aPostfix;

                    if (
                        !((IntegerColumn*)aFrame->typeColumns.at(currentRow).column)->mIsAutoInc
                        &&
                        !((IntegerColumn*)aFrame->typeColumns.at(currentRow).column)->mSplitRows
                       )
                    {
                        aPrefix=((IntegerColumn*)aFrame->typeColumns.at(currentRow).column)->mPrefix;
                        aPostfix=((IntegerColumn*)aFrame->typeColumns.at(currentRow).column)->mPostfix;
                    }

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

                ui->extendedListComboBox->addItems(aItems);
            }
        }
    }

    int index=ui->extendedListComboBox->findText(currentDefault);

    if (index>=0)
    {
        ui->extendedListComboBox->setCurrentIndex(index);
    }
    else
    {
        ui->extendedListComboBox->setEditText(currentDefault);
    }
}

void ColumnEditDialog::on_functionButton_clicked()
{
    FunctionDialog dialog(mTable, this);
    dialog.ui->expressionEdit->setText(ui->expressionEdit->text());

    if (dialog.exec())
    {
        ui->expressionEdit->setText(dialog.ui->expressionEdit->text());
    }
}

void ColumnEditDialog::on_leftButton_clicked()
{
    applyChanges();

    mColumnIndex--;
    mTableWidget->setCurrentCell(0, mColumnIndex);

    startEditing();
}

void ColumnEditDialog::on_rightButton_clicked()
{
    applyChanges();

    mColumnIndex++;
    mTableWidget->setCurrentCell(0, mColumnIndex);

    startEditing();
}

void ColumnEditDialog::on_addToolButton_clicked()
{
    ConditionalFormatFrame *aCondition = new ConditionalFormatFrame(mTable, this);
    ui->conditionsVerticalLayout->addWidget(aCondition);

    aCondition->ui->downToolButton->setEnabled(false);
    aCondition->ui->upToolButton->setEnabled(ui->conditionsVerticalLayout->count()>1);

    if (aCondition->ui->upToolButton->isEnabled())
    {
        ((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(ui->conditionsVerticalLayout->count()-2)->widget()))->ui->downToolButton->setEnabled(true);
    }

    connect(aCondition, SIGNAL(delRequested(ConditionalFormatFrame*)), this, SLOT(delRequested(ConditionalFormatFrame*)));
    connect(aCondition, SIGNAL(upRequested(ConditionalFormatFrame*)), this, SLOT(upRequested(ConditionalFormatFrame*)));
    connect(aCondition, SIGNAL(downRequested(ConditionalFormatFrame*)), this, SLOT(downRequested(ConditionalFormatFrame*)));
}

void ColumnEditDialog::delRequested(ConditionalFormatFrame *condition)
{
    int index=ui->conditionsVerticalLayout->indexOf(condition);

    if (ui->conditionsVerticalLayout->count()>1)
    {
        if (index==0)
        {
            ((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(1)->widget()))->ui->upToolButton->setEnabled(false);
        }
        else
        if (index==ui->conditionsVerticalLayout->count()-1)
        {
            ((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(ui->conditionsVerticalLayout->count()-2)->widget()))->ui->downToolButton->setEnabled(false);
        }
    }

    ui->conditionsVerticalLayout->removeWidget(condition);
    delete condition;
}

void ColumnEditDialog::upRequested(ConditionalFormatFrame *condition)
{
    int index=ui->conditionsVerticalLayout->indexOf(condition);

    ConditionalFormatFrame *first = ((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(index)->widget()));
    ConditionalFormatFrame *second = ((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(index-1)->widget()));

    ui->conditionsVerticalLayout->removeWidget(first);
    ui->conditionsVerticalLayout->insertWidget(index-1, first);

    bool temp;

    temp=first->ui->upToolButton->isEnabled();
    first->ui->upToolButton->setEnabled(second->ui->upToolButton->isEnabled());
    second->ui->upToolButton->setEnabled(temp);

    temp=first->ui->downToolButton->isEnabled();
    first->ui->downToolButton->setEnabled(second->ui->downToolButton->isEnabled());
    second->ui->downToolButton->setEnabled(temp);
}

void ColumnEditDialog::downRequested(ConditionalFormatFrame *condition)
{
    int index=ui->conditionsVerticalLayout->indexOf(condition);

    ConditionalFormatFrame *first = ((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(index)->widget()));
    ConditionalFormatFrame *second = ((ConditionalFormatFrame *)(ui->conditionsVerticalLayout->itemAt(index+1)->widget()));

    ui->conditionsVerticalLayout->removeWidget(first);
    ui->conditionsVerticalLayout->insertWidget(index+1, first);

    bool temp;

    temp=first->ui->upToolButton->isEnabled();
    first->ui->upToolButton->setEnabled(second->ui->upToolButton->isEnabled());
    second->ui->upToolButton->setEnabled(temp);

    temp=first->ui->downToolButton->isEnabled();
    first->ui->downToolButton->setEnabled(second->ui->downToolButton->isEnabled());
    second->ui->downToolButton->setEnabled(temp);
}
