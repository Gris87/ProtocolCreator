#include "src/other/global.h"

VariableExtendedListFrame::VariableExtendedListFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableExtendedListFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("Расширенный список");
    ui->varNameEdit->setText("ExtendedList");

    mTableAlignment=Qt::AlignCenter;
    mTableOffset=0;
    middleRowFontString="";
    middleRowAlignment=Qt::AlignTop | Qt::AlignLeft;
    middleRowBackgroundColor=QColor(255, 255, 255);
    middleRowTextColor=QColor(0, 0, 0);

    ui->dataTableWidget->setStyleSheet( "QTableView { gridline-color: black; }" );
    ui->dataTableWidget->mTable=this;
}

VariableExtendedListFrame::~VariableExtendedListFrame()
{
    for (int i=0; i<typeColumns.length(); i++)
    {
        delete typeColumns.at(i).column;
    }

    delete ui;
}

QString VariableExtendedListFrame::name()
{
    return ui->nameEdit->text();
}

QString VariableExtendedListFrame::variableName()
{
    return ui->varNameEdit->text();
}

void VariableExtendedListFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("VarExtendedList");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    bool aUsed=ui->useCheckBox->isChecked();

    aStream << QString("Used");
    aStream << aUsed;

    bool aEdit=isEditable();

    aStream << QString("Edit");
    aStream << aEdit;

    bool aLock=!ui->userWidget->isEnabled();

    aStream << QString("Lock");
    aStream << aLock;

    int intTableAlignment=(int)mTableAlignment;

    aStream << QString("TableAlignment");
    aStream << intTableAlignment;

    aStream << QString("TableOffset");
    aStream << mTableOffset;

    int headerRowCount=headerCells.length();
    int headerColCount=headerRowCount==0? 0 : headerCells.at(0).length();

    aStream << QString("Header");
    aStream << headerRowCount;
    aStream << headerColCount;

    aStream << QString("ColumnWidth");

    for (int i=0; i<headerColCount; i++)
    {
        aStream << headerColumnWidths.at(i);
    }

    aStream << QString("Cells");

    for (int i=0; i<headerRowCount; i++)
    {
        for (int j=0; j<headerColCount; j++)
        {
            STableCell *aCell=&headerCells[i][j];

            aStream << QString("SpanX");
            aStream << aCell->spanX;

            aStream << QString("SpanY");
            aStream << aCell->spanY;

            aStream << QString("Font");
            aStream << aCell->fontString;

            aStream << QString("Text");
            aStream << aCell->text;

            aStream << QString("Alignment");
            aStream << aCell->alignment;

            aStream << QString("Background");
            aStream << aCell->backgroundColorR;
            aStream << aCell->backgroundColorG;
            aStream << aCell->backgroundColorB;

            aStream << QString("TextColor");
            aStream << aCell->textColorR;
            aStream << aCell->textColorG;
            aStream << aCell->textColorB;

            aStream << QString("CellEnd");
        }
    }

    aStream << QString("HeaderEnd");

    aStream << QString("ColTypes");

    int typesCount=typeColumns.length();
    aStream << typesCount;

    aStream << QString("ColumnWidth");

    for (int i=0; i<typesCount; i++)
    {
        aStream << typeColumnWidths.at(i);
    }

    aStream << QString("Types");

    for (int i=0; i<typesCount; i++)
    {
        STableColumn *aColumn=&typeColumns[i];

        aStream << QString("Name");
        aStream << aColumn->name;

        aStream << QString("Column");
        aColumn->column->saveToStream(aStream);

        aStream << QString("LeftOffset");
        aStream << aColumn->leftOffset;

        aStream << QString("RightOffset");
        aStream << aColumn->rightOffset;

        aStream << QString("TotalOffset");
        aStream << aColumn->totalOffset;

        aStream << QString("Font");
        aStream << aColumn->fontString;

        aStream << QString("Alignment");
        aStream << aColumn->alignment;

        aStream << QString("Background");
        aStream << aColumn->backgroundColorR;
        aStream << aColumn->backgroundColorG;
        aStream << aColumn->backgroundColorB;

        aStream << QString("TextColor");
        aStream << aColumn->textColorR;
        aStream << aColumn->textColorG;
        aStream << aColumn->textColorB;

        aStream << QString("TypeEnd");
    }

    aStream << QString("ColTypesEnd");

    aStream << QString("MiddleFont");
    aStream << middleRowFontString;

    aStream << QString("MiddleAlignment");
    aStream << middleRowAlignment;

    aStream << QString("MiddleBackground");
    aStream << (quint8)middleRowBackgroundColor.red();
    aStream << (quint8)middleRowBackgroundColor.green();
    aStream << (quint8)middleRowBackgroundColor.blue();

    aStream << QString("MiddleTextColor");
    aStream << (quint8)middleRowTextColor.red();
    aStream << (quint8)middleRowTextColor.green();
    aStream << (quint8)middleRowTextColor.blue();

    aStream << QString("VarEnd");
}

void VariableExtendedListFrame::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="Used")
        {
            bool aUsed;

            aStream >> aUsed;
            ui->useCheckBox->setChecked(aUsed);
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
            ui->userWidget->setEnabled(!aLock);

            updateLock();
        }
        else
        if (aMagicWord=="TableAlignment")
        {
            int intTableAlignment;

            aStream >> intTableAlignment;

            mTableAlignment=(Qt::AlignmentFlag)intTableAlignment;
        }
        else
        if (aMagicWord=="TableOffset")
        {
            aStream >> mTableOffset;
        }
        else
        if (aMagicWord=="Header")
        {
            int headerRowCount;
            int headerColCount;

            aStream >> headerRowCount;
            aStream >> headerColCount;

            while (!aStream.atEnd())
            {
                aStream >> aMagicWord;

                if (aMagicWord=="ColumnWidth")
                {
                    int aColumnWidth;

                    for (int i=0; i<headerColCount; i++)
                    {
                        aStream >> aColumnWidth;

                        headerColumnWidths.append(aColumnWidth);
                    }
                }
                else
                if (aMagicWord=="Cells")
                {
                    QList<STableCell> aNewRow;

                    for (int i=0; i<headerRowCount; i++)
                    {
                        headerCells.append(aNewRow);

                        for (int j=0; j<headerColCount; j++)
                        {
                            STableCell aCell;

                            while (!aStream.atEnd())
                            {
                                aStream >> aMagicWord;

                                if (aMagicWord=="SpanX")
                                {
                                    aStream >> aCell.spanX;
                                }
                                else
                                if (aMagicWord=="SpanY")
                                {
                                    aStream >> aCell.spanY;
                                }
                                else
                                if (aMagicWord=="Font")
                                {
                                    aStream >> aCell.fontString;
                                }
                                else
                                if (aMagicWord=="Text")
                                {
                                    aStream >> aCell.text;
                                }
                                else
                                if (aMagicWord=="Alignment")
                                {
                                    aStream >> aCell.alignment;
                                }
                                else
                                if (aMagicWord=="Background")
                                {
                                    aStream >> aCell.backgroundColorR;
                                    aStream >> aCell.backgroundColorG;
                                    aStream >> aCell.backgroundColorB;
                                }
                                else
                                if (aMagicWord=="TextColor")
                                {
                                    aStream >> aCell.textColorR;
                                    aStream >> aCell.textColorG;
                                    aStream >> aCell.textColorB;
                                }
                                else
                                if (aMagicWord=="CellEnd")
                                {
                                    break;
                                }
                            }

                            headerCells[i].append(aCell);
                        }
                    }
                }
                else
                if (aMagicWord=="HeaderEnd")
                {
                    break;
                }
            }
        }
        else
        if (aMagicWord=="ColTypes")
        {
            int aColumnCount;
            aStream >> aColumnCount;

            while (!aStream.atEnd())
            {
                aStream >> aMagicWord;

                if (aMagicWord=="ColumnWidth")
                {
                    int aColumnWidth;

                    for (int i=0; i<aColumnCount; i++)
                    {
                        aStream >> aColumnWidth;

                        typeColumnWidths.append(aColumnWidth);
                    }
                }
                else
                if (aMagicWord=="Types")
                {
                    for (int i=0; i<aColumnCount; i++)
                    {
                        STableColumn aColumn;

                        while (!aStream.atEnd())
                        {
                            aStream >> aMagicWord;

                            if (aMagicWord=="Name")
                            {
                                aStream >> aColumn.name;
                            }
                            else
                            if (aMagicWord=="Column")
                            {
                                aStream >> aMagicWord;

                                ColumnType *aTypeColumn=0;

                                if (aMagicWord=="ColInteger")
                                {
                                    aTypeColumn=new IntegerColumn();
                                }
                                else
                                if (aMagicWord=="ColString")
                                {
                                    aTypeColumn=new StringColumn();
                                }
                                else
                                if (aMagicWord=="ColBool")
                                {
                                    aTypeColumn=new BoolColumn();
                                }
                                else
                                if (aMagicWord=="ColDate")
                                {
                                    aTypeColumn=new DateColumn();
                                }
                                else
                                if (aMagicWord=="ColTime")
                                {
                                    aTypeColumn=new TimeColumn();
                                }
                                else
                                {
                                    throw "Unknown type column";
                                }

                                aTypeColumn->loadFromStream(aStream);

                                aColumn.column=aTypeColumn;
                            }
                            else
                            if (aMagicWord=="LeftOffset")
                            {
                                aStream >> aColumn.leftOffset;
                            }
                            else
                            if (aMagicWord=="RightOffset")
                            {
                                aStream >> aColumn.rightOffset;
                            }
                            else
                            if (aMagicWord=="TotalOffset")
                            {
                                aStream >> aColumn.totalOffset;
                            }
                            else
                            if (aMagicWord=="Font")
                            {
                                aStream >> aColumn.fontString;
                            }
                            if (aMagicWord=="Alignment")
                            {
                                aStream >> aColumn.alignment;
                            }
                            if (aMagicWord=="Background")
                            {
                                aStream >> aColumn.backgroundColorR;
                                aStream >> aColumn.backgroundColorG;
                                aStream >> aColumn.backgroundColorB;
                            }
                            if (aMagicWord=="TextColor")
                            {
                                aStream >> aColumn.textColorR;
                                aStream >> aColumn.textColorG;
                                aStream >> aColumn.textColorB;
                            }
                            else
                            if (aMagicWord=="TypeEnd")
                            {
                                break;
                            }
                        }

                        typeColumns.append(aColumn);
                    }
                }
                else
                if (aMagicWord=="ColTypesEnd")
                {
                    break;
                }
            }
        }
        else
        if (aMagicWord=="MiddleFont")
        {
            aStream >> middleRowFontString;
        }
        else
        if (aMagicWord=="MiddleAlignment")
        {
            aStream >> middleRowAlignment;
        }
        else
        if (aMagicWord=="MiddleBackground")
        {
            quint8 r,g,b;

            aStream >> r;
            aStream >> g;
            aStream >> b;

            middleRowBackgroundColor=QColor(r, g, b);
        }
        else
        if (aMagicWord=="MiddleTextColor")
        {
            quint8 r,g,b;

            aStream >> r;
            aStream >> g;
            aStream >> b;

            middleRowTextColor=QColor(r, g, b);
        }
        else
        if (aMagicWord=="VarEnd")
        {
            break;
        }
    }
}

void VariableExtendedListFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
    on_useCheckBox_toggled(ui->useCheckBox->isChecked());
}

void VariableExtendedListFrame::setUpDownEnabled(bool aUpEnabled, bool aDownEnabled)
{
    ui->upButton->setEnabled(aUpEnabled);
    ui->downButton->setEnabled(aDownEnabled);
}

void VariableExtendedListFrame::on_switchButton_clicked()
{
    emit switchPressed(this);
}

void VariableExtendedListFrame::on_upButton_clicked()
{
    emit upPressed(this);
}

void VariableExtendedListFrame::on_downButton_clicked()
{
    emit downPressed(this);
}

void VariableExtendedListFrame::on_copyButton_clicked()
{
    emit copyPressed(this);
}

void VariableExtendedListFrame::on_deleteButton_clicked()
{
    emit deletePressed(this);
}

void VariableExtendedListFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->useCheckBox->setText(aName);
    ui->titleLabel->setText(aName);
}

void VariableExtendedListFrame::on_useCheckBox_toggled(bool checked)
{
    ui->userWidget->setVisible(checked && (isAdmin || isEditable()));
}

void VariableExtendedListFrame::on_lockButton_clicked()
{
    ui->userWidget->setEnabled(!ui->userWidget->isEnabled());

    updateLock();
}

void VariableExtendedListFrame::updateLock()
{
    ui->configButton->setEnabled(ui->userWidget->isEnabled());
    ui->expandButton->setEnabled(ui->userWidget->isEnabled());

    if (ui->userWidget->isEnabled())
    {
        ui->lockButton->setIcon(QIcon(":/images/Unlock.png"));
    }
    else
    {
        ui->lockButton->setIcon(QIcon(":/images/Lock.png"));
    }
}

void VariableExtendedListFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool VariableExtendedListFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

void VariableExtendedListFrame::checkForErrors(QStringList &aErrorList)
{
    PageComponent::checkForErrors(aErrorList);
}

QVariant VariableExtendedListFrame::calculate()
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate();

    return calculationResult;
}

void VariableExtendedListFrame::on_expandButton_clicked()
{
    FullDialog dialog(this);
    dialog.setWindowTitle(ui->nameEdit->text());

    //------------------------------------------

    ui->mainVerticalLayout->removeWidget(ui->expandedWidget);

    ui->expandButton->setVisible(false);
    ui->lockButton->setVisible(false);

    bool wasTable=ui->useCheckBox->isVisible();
    bool wasVisible=ui->useCheckBox->isChecked();
    ui->useCheckBox->setChecked(true);
    ui->useCheckBox->setVisible(false);
    ui->titleLabel->setVisible(false);

    ui->expandedWidget->setParent(&dialog);
    ui->expandedWidget->show();
    dialog.ui->verticalLayout->addWidget(ui->expandedWidget);

    dialog.exec();

    //------------------------------------------

    ui->expandButton->setVisible(true);
    ui->lockButton->setVisible(true);

    ui->useCheckBox->setChecked(wasVisible);
    ui->useCheckBox->setVisible(wasTable);
    ui->titleLabel->setVisible(!wasTable);

    ui->expandedWidget->setParent(this);
    ui->expandedWidget->show();
    ui->mainVerticalLayout->addWidget(ui->expandedWidget);
}

void VariableExtendedListFrame::on_configButton_clicked()
{
    TableEditDialog dialog(this, this);
    dialog.exec();
}

void VariableExtendedListFrame::setItemsForRow(int row)
{
    for (int i=0; i<ui->dataTableWidget->columnCount(); i++)
    {
        QTableWidgetItem *aItem=new QTableWidgetItem();

        QFont aFont;
        aFont.fromString(typeColumns.at(i).fontString);
        aItem->setFont(aFont);

        aItem->setTextAlignment(typeColumns.at(i).alignment);
        aItem->setBackground(QBrush(QColor(typeColumns.at(i).backgroundColorR, typeColumns.at(i).backgroundColorG, typeColumns.at(i).backgroundColorB)));
        aItem->setTextColor(QColor(typeColumns.at(i).textColorR, typeColumns.at(i).textColorG, typeColumns.at(i).textColorB));

        switch (typeColumns.at(i).column->type())
        {
            case ctInteger:
            {
                if (((IntegerColumn*)typeColumns.at(i).column)->mIsAutoInc)
                {
                    int id=1;

                    for (int j=0; j<row; j++)
                    {
                        if (ui->dataTableWidget->itemDelegateForRow(j)==0)
                        {
                            id++;
                        }
                    }

                    aItem->setText(QString::number(id));

                    for (int j=row+1; j<ui->dataTableWidget->rowCount(); j++)
                    {
                        if (ui->dataTableWidget->itemDelegateForRow(j)==0)
                        {
                            id++;
                            ui->dataTableWidget->item(j, 0)->setText(QString::number(id));
                        }
                    }
                }
                else
                {
                    aItem->setText(QString::number((((IntegerColumn*)typeColumns.at(i).column)->mDefaultValue)));
                }
            }
            break;
            case ctString:
            {
                aItem->setText((((StringColumn*)typeColumns.at(i).column)->mDefaultValue));
            }
            break;
            case ctBool:
            {
                if (((BoolColumn*)typeColumns.at(i).column)->mDefaultValue)
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
                aItem->setText(((DateColumn*)typeColumns.at(i).column)->mDefaultValue.toString("dd.MM.yyyy"));
            }
            break;
            case ctTime:
            {
                aItem->setText(((TimeColumn*)typeColumns.at(i).column)->mDefaultValue.toString("hh:mm:ss"));
            }
            break;
            case ctList:
            {
                aItem->setText("");
            }
            break;
            case ctExtendedList:
            {
                aItem->setText("");
            }
            break;
            case ctExpression:
            {
                aItem->setText("");
            }
            break;
            default:
            break;
        }

        ui->dataTableWidget->setItem(row, i, aItem);
    }
}

void VariableExtendedListFrame::setItemsForMiddleRow(int row)
{
    ui->dataTableWidget->setRowCount(row+1);

    for (int i=0; i<ui->dataTableWidget->columnCount(); i++)
    {
        QTableWidgetItem *aItem=new QTableWidgetItem();

        QFont aFont;
        aFont.fromString(middleRowFontString);
        aItem->setFont(aFont);

        aItem->setTextAlignment(middleRowAlignment);
        aItem->setBackground(QBrush(middleRowBackgroundColor));
        aItem->setTextColor(middleRowTextColor);

        ui->dataTableWidget->setItem(row, i, aItem);
    }

    ui->dataTableWidget->setSpan(row, 0, 1, ui->dataTableWidget->columnCount());
    ui->dataTableWidget->setItemDelegateForRow(row, new StringDelegate(this));
}

void VariableExtendedListFrame::on_addRowButton_clicked()
{
    ui->dataTableWidget->setRowCount(ui->dataTableWidget->rowCount()+1);

    setItemsForRow(ui->dataTableWidget->rowCount()-1);

    ui->dataTableWidget->scrollToBottom();
}

void VariableExtendedListFrame::on_addMiddleRowButton_clicked()
{
    ui->dataTableWidget->setRowCount(ui->dataTableWidget->rowCount()+1);

    setItemsForMiddleRow(ui->dataTableWidget->rowCount()-1);

    ui->dataTableWidget->scrollToBottom();
}

void VariableExtendedListFrame::on_deleteRowButton_clicked()
{
    QList<QTableWidgetSelectionRange> aRanges=ui->dataTableWidget->selectedRanges();

    if (aRanges.length()==0)
    {
        QMessageBox::information(this, protocolCreatorVersion, "Выберите строку");
        return;
    }

    QList<int> aRows;

    for (int i=0; i<aRanges.length(); i++)
    {
        for (int j=aRanges.at(i).bottomRow(); j>=aRanges.at(i).topRow(); j--)
        {
            if (!aRows.contains(j))
            {
                aRows.append(j);
            }
        }
    }

    for (int e=0; e<aRows.length()-1; e++)
    {
        int max=aRows.at(e);
        int maxIndex=e;

        for (int i=e+1; i<aRows.length(); i++)
        {
            if (aRows.at(i)>max)
            {
                max=aRows.at(i);
                maxIndex=i;
            }
        }

        aRows.swap(e, maxIndex);
    }

    for (int i=0; i<aRows.length(); i++)
    {
        int realRow=aRows.at(i);

        if (ui->dataTableWidget->itemDelegateForRow(realRow))
        {
            delete ui->dataTableWidget->itemDelegateForRow(realRow);
        }

        for (int j=realRow; j<ui->dataTableWidget->rowCount()-1; j++)
        {
            ui->dataTableWidget->setItemDelegateForRow(j, ui->dataTableWidget->itemDelegateForRow(j+1));
        }

        ui->dataTableWidget->removeRow(realRow);
    }
}
