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

void VariableExtendedListFrame::on_configButton_clicked()
{
    TableEditDialog dialog(this, this);
    dialog.exec();
}
