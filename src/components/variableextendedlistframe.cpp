#include "src/other/global.h"

VariableExtendedListFrame::VariableExtendedListFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableExtendedListFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("Расширенный список");
    ui->varNameEdit->setText("ExtendedList");

    mIsTable=false;

    mTableAlignment=Qt::AlignCenter;
    mTableOffset=0;
    middleRowFontString="";
    middleRowAlignment=Qt::AlignTop | Qt::AlignLeft;
    middleRowBackgroundColor=QColor(255, 255, 255);
    middleRowTextColor=QColor(0, 0, 0);

    mCellAlignmentWidget=new CellAlignmentWidget(this);
    mCellAlignmentWidget->setWindowFlags(Qt::Popup);
    connect(mCellAlignmentWidget->ui->topLeftButton,     SIGNAL(clicked()), this, SLOT(tableCellAlignTopLeft()));
    connect(mCellAlignmentWidget->ui->topButton,         SIGNAL(clicked()), this, SLOT(tableCellAlignTop()));
    connect(mCellAlignmentWidget->ui->topRightButton,    SIGNAL(clicked()), this, SLOT(tableCellAlignTopRight()));
    connect(mCellAlignmentWidget->ui->leftButton,        SIGNAL(clicked()), this, SLOT(tableCellAlignLeft()));
    connect(mCellAlignmentWidget->ui->centerButton,      SIGNAL(clicked()), this, SLOT(tableCellAlignCenter()));
    connect(mCellAlignmentWidget->ui->rightButton,       SIGNAL(clicked()), this, SLOT(tableCellAlignRight()));
    connect(mCellAlignmentWidget->ui->bottomLeftButton,  SIGNAL(clicked()), this, SLOT(tableCellAlignBottomLeft()));
    connect(mCellAlignmentWidget->ui->bottomButton,      SIGNAL(clicked()), this, SLOT(tableCellAlignBottom()));
    connect(mCellAlignmentWidget->ui->bottomRightButton, SIGNAL(clicked()), this, SLOT(tableCellAlignBottomRight()));

    ui->dataTableWidget->setStyleSheet( "QTableView { gridline-color: black; }" );
    ui->dataTableWidget->verticalHeader()->setMovable(true);
    ui->dataTableWidget->mTable=this;

    connect(ui->dataTableWidget->verticalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(dataTableHeaderMove(int,int,int)));
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

    aStream << QString("LinkForMiddleRow");
    aStream << mLinkForMiddleRow;

    aStream << QString("LinkForAnotherList");
    aStream << mLinkForAnotherList;

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

        aStream << QString("Visible");
        aStream << aColumn->visible;

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

    if (typesCount>0)
    {
        aStream << QString("Data");

        aStream << QString("ColumnWidth");

        for (int i=0; i<typesCount; i++)
        {
            aStream << ui->dataTableWidget->columnWidth(i);
        }

        aStream << QString("Rows");

        int rowsCount=ui->dataTableWidget->rowCount();
        aStream << rowsCount;

        QTableWidgetItem *aItem;
        QColor aColor;

        for (int i=0; i<rowsCount; i++)
        {
            bool isMiddleRow=ui->dataTableWidget->itemDelegateForRow(i)!=0;
            int stop;

            if (isMiddleRow)
            {
                aStream << QString("MiddleRow");
                stop=1;
            }
            else
            {
                aStream << QString("Row");
                stop=typesCount;
            }

            for (int j=0; j<stop; j++)
            {
                aItem=ui->dataTableWidget->item(i, j);

                aStream << QString("Font");
                aStream << aItem->font().toString();

                aStream << QString("Alignment");
                aStream << aItem->textAlignment();

                aColor=aItem->background().color();

                aStream << QString("Background");
                aStream << (quint8)aColor.red();
                aStream << (quint8)aColor.green();
                aStream << (quint8)aColor.blue();

                aColor=aItem->textColor();

                aStream << QString("TextColor");
                aStream << (quint8)aColor.red();
                aStream << (quint8)aColor.green();
                aStream << (quint8)aColor.blue();

                if (aItem->text()!="")
                {
                    aStream << QString("Text");
                    aStream << aItem->text();
                }

                if (!isMiddleRow && typeColumns.at(j).column->type()==ctBool)
                {
                    aStream << QString("Checked");
                    bool isChecked=aItem->checkState()==Qt::Checked;
                    aStream << isChecked;
                }

                aStream << QString("CellEnd");
            }
        }

        aStream << QString("DataEnd");
    }

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
        if (aMagicWord=="LinkForMiddleRow")
        {
            aStream >> mLinkForMiddleRow;
        }
        else
        if (aMagicWord=="LinkForAnotherList")
        {
            aStream >> mLinkForAnotherList;
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

            ui->dataTableWidget->setColumnCount(aColumnCount);

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
                                ui->dataTableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(aColumn.name));
                            }
                            else
                            if (aMagicWord=="Visible")
                            {
                                aStream >> aColumn.visible;
                                ui->dataTableWidget->horizontalHeaderItem(i)->setTextColor(aColumn.visible? QColor(0, 0, 0) : QColor(128, 128, 128));
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
                                if (aMagicWord=="ColList")
                                {
                                    aTypeColumn=new ListColumn();
                                }
                                else
                                if (aMagicWord=="ColExtList")
                                {
                                    aTypeColumn=new ExtendedListColumn();
                                }
                                else
                                if (aMagicWord=="ColExpression")
                                {
                                    aTypeColumn=new ExpressionColumn();
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
                            else
                            if (aMagicWord=="Alignment")
                            {
                                aStream >> aColumn.alignment;
                            }
                            else
                            if (aMagicWord=="Background")
                            {
                                aStream >> aColumn.backgroundColorR;
                                aStream >> aColumn.backgroundColorG;
                                aStream >> aColumn.backgroundColorB;
                            }
                            else
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
        if (aMagicWord=="Data")
        {
            while (!aStream.atEnd())
            {
                aStream >> aMagicWord;

                if (aMagicWord=="ColumnWidth")
                {
                    int aColumnWidth;

                    for (int i=0; i<ui->dataTableWidget->columnCount(); i++)
                    {
                        aStream >> aColumnWidth;

                        ui->dataTableWidget->setColumnWidth(i, aColumnWidth);
                    }
                }
                else
                if (aMagicWord=="Rows")
                {
                    int aRowCount;
                    aStream >> aRowCount;

                    ui->dataTableWidget->setRowCount(aRowCount);

                    QTableWidgetItem *aItem=0;
                    QString isMiddleRow;

                    for (int i=0; i<aRowCount; i++)
                    {
                        aStream >> isMiddleRow;

                        int stop;

                        if (isMiddleRow=="MiddleRow")
                        {
                            stop=1;
                        }
                        else
                        {
                            stop=typeColumns.length();
                        }

                        for (int j=0; j<stop; j++)
                        {
                            aItem=new QTableWidgetItem();

                            while (!aStream.atEnd())
                            {
                                aStream >> aMagicWord;

                                if (aMagicWord=="Font")
                                {
                                    QString fontString;
                                    aStream >> fontString;

                                    QFont aFont;
                                    aFont.fromString(fontString);

                                    aItem->setFont(aFont);
                                }
                                else
                                if (aMagicWord=="Alignment")
                                {
                                    int textAlignment;
                                    aStream >> textAlignment;

                                    aItem->setTextAlignment(textAlignment);
                                }
                                else
                                if (aMagicWord=="Background")
                                {
                                    quint8 r,g,b;

                                    aStream >> r;
                                    aStream >> g;
                                    aStream >> b;

                                    aItem->setBackground(QBrush(QColor(r, g, b)));
                                }
                                else
                                if (aMagicWord=="TextColor")
                                {
                                    quint8 r,g,b;

                                    aStream >> r;
                                    aStream >> g;
                                    aStream >> b;

                                    aItem->setTextColor(QColor(r, g, b));
                                }
                                else
                                if (aMagicWord=="Text")
                                {
                                    QString aValue;

                                    aStream >> aValue;

                                    aItem->setText(aValue);
                                }
                                else
                                if (aMagicWord=="Checked")
                                {
                                    bool isChecked;

                                    aStream >> isChecked;

                                    aItem->setCheckState(isChecked? Qt::Checked : Qt::Unchecked);
                                }
                                else
                                if (aMagicWord=="CellEnd")
                                {
                                    break;
                                }
                            }

                            ui->dataTableWidget->setItem(i, j, aItem);
                        }

                        if (isMiddleRow=="MiddleRow")
                        {
                            for (int j=1; j<typeColumns.length(); j++)
                            {
                                ui->dataTableWidget->setItem(i, j, aItem->clone());
                            }

                            ui->dataTableWidget->setSpan(i, 0, 1, typeColumns.length());

                            if (mLinkForMiddleRow=="")
                            {
                                ui->dataTableWidget->setItemDelegateForRow(i, new StringDelegate(this));
                            }
                            else
                            {
                                ui->dataTableWidget->setItemDelegateForRow(i, new ListDelegate(mLinkForMiddleRow, this));
                            }
                        }
                    }
                }
                else
                if (aMagicWord=="DataEnd")
                {
                    break;
                }
            }
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
    ui->userWidget->setVisible(ui->userWidget->isEnabled() && checked && (isAdmin || isEditable()));
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

    ui->userWidget->setVisible(ui->userWidget->isEnabled() && ui->useCheckBox->isChecked() && (isAdmin || isEditable()));
}

void VariableExtendedListFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool VariableExtendedListFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

void VariableExtendedListFrame::checkLink(QStringList &aErrorList, QString aLink)
{
    if (aLink!="")
    {
        int index=aLink.indexOf(".");

        if (index>=0)
        {
            QString aSectionName=aLink.left(index);
            aLink=aLink.mid(index+1);

            QString aVarName;
            QString aColumnIndex="";
            index=aLink.indexOf("[");

            if (index>=0)
            {
                aVarName=aLink.left(index);
                aLink=aLink.mid(index+1);
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

                if (!ok || aColumnID<0 || aColumnID>=aExtFrame->typeColumns.length())
                {
                    if (ok)
                    {
                        if (aSectionName=="Global")
                        {
                            aErrorList.append("Hint: Индекс неправильный у глобальной переменной \""+aVarName+"\"");
                        }
                        else
                        {
                            aErrorList.append("Hint: Индекс неправильный у переменной \""+aVarName+"\" в разделе \""+aSectionName+"\"");
                        }
                    }
                    else
                    {
                        if (aSectionName=="Global")
                        {
                            aErrorList.append("Hint: Индекс столбца не число у глобальной переменной \""+aVarName+"\"");
                        }
                        else
                        {
                            aErrorList.append("Hint: Индекс столбца не число у переменной \""+aVarName+"\" в разделе \""+aSectionName+"\"");
                        }
                    }
                }
            }
            else
            if (aFrame==0)
            {
                if (aSectionName=="Global")
                {
                    aErrorList.append("Hint: Не найдена глобальная переменная \""+aVarName+"\"");
                }
                else
                {
                    aErrorList.append("Hint: Не найдена переменная \""+aVarName+"\" в разделе \""+aSectionName+"\"");
                }
            }
        }
        else
        {
            aErrorList.append("Hint: Не найдена точка для определения связи со списком");
        }
    }
}

void VariableExtendedListFrame::checkForErrors(QStringList &aErrorList)
{
    PageComponent::checkForErrors(aErrorList);

    if (mIsTable)
    {
        int visibleColumns=0;

        for (int i=0; i<typeColumns.length(); i++)
        {
            if (typeColumns.at(i).visible)
            {
                visibleColumns++;
            }
        }

        if (visibleColumns!=headerColumnWidths.length())
        {
            aErrorList.append("Error: Количество видимых столбцов в таблице не совпадает с количеством столбцов в заголовке");
        }
    }

    checkLink(aErrorList, mLinkForMiddleRow);
    checkLink(aErrorList, mLinkForAnotherList);

    for (int i=0; i<typeColumns.length(); i++)
    {
        ColumnType *aColumnType=typeColumns.at(i).column;

        QString aLink="";

        if (aColumnType->type()==ctList)
        {
            aLink=((ListColumn*)aColumnType)->mLinkComponent;
        }
        else
        if (aColumnType->type()==ctExtendedList)
        {
            aLink=((ExtendedListColumn*)aColumnType)->mLinkComponent;
        }

        checkLink(aErrorList, aLink);
    }
}

QVariant VariableExtendedListFrame::calculate()
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate();

    mCellResults.clear();

    for (int i=0; i<ui->dataTableWidget->rowCount(); i++)
    {
        QList<QVariant> aRow;

        for (int j=0; j<ui->dataTableWidget->columnCount(); j++)
        {
            QVariant aCellValue;
            aRow.append(aCellValue);
        }

        mCellResults.append(aRow);
    }

//----------------------------------------------

    if (mCellResults.length()>0)
    {
        for (int j=0; j<ui->dataTableWidget->columnCount(); j++)
        {
            switch(typeColumns.at(j).column->type())
            {
                case ctInteger:
                {
                    int removeBefore=0;
                    int removeAfter=0;

                    if (!((IntegerColumn*)typeColumns.at(j).column)->mIsAutoInc)
                    {
                        removeBefore=((IntegerColumn*)typeColumns.at(j).column)->mPrefix.length();
                        removeAfter=((IntegerColumn*)typeColumns.at(j).column)->mPostfix.length();
                    }

                    for (int i=0; i<ui->dataTableWidget->rowCount(); i++)
                    {
                        if (ui->dataTableWidget->itemDelegateForRow(i))
                        {
                            mCellResults[i][j]=ui->dataTableWidget->item(i,j)->text();
                        }
                        else
                        {
                            QString aText=ui->dataTableWidget->item(i,j)->text();

                            aText.remove(aText.length()-removeAfter, removeAfter);
                            aText.remove(0, removeBefore);

                            mCellResults[i][j]=aText.toDouble();
                        }
                    }
                }
                break;
                case ctString:
                case ctList:
                case ctExtendedList:
                {
                    for (int i=0; i<ui->dataTableWidget->rowCount(); i++)
                    {
                        mCellResults[i][j]=ui->dataTableWidget->item(i,j)->text();
                    }
                }
                break;
                case ctBool:
                {
                    for (int i=0; i<ui->dataTableWidget->rowCount(); i++)
                    {
                        if (ui->dataTableWidget->itemDelegateForRow(i))
                        {
                            mCellResults[i][j]=ui->dataTableWidget->item(i,j)->text();
                        }
                        else
                        {
                            if (ui->dataTableWidget->item(i,j)->checkState()==Qt::Checked)
                            {
                                mCellResults[i][j]=true;
                            }
                            else
                            {
                                mCellResults[i][j]=false;
                            }
                        }
                    }
                }
                break;
                case ctDate:
                {
                    for (int i=0; i<ui->dataTableWidget->rowCount(); i++)
                    {
                        if (ui->dataTableWidget->itemDelegateForRow(i))
                        {
                            mCellResults[i][j]=ui->dataTableWidget->item(i,j)->text();
                        }
                        else
                        {
                            mCellResults[i][j]=QDate::fromString(ui->dataTableWidget->item(i,j)->text(), "dd.MM.yyyy");
                        }
                    }
                }
                break;
                case ctTime:
                {
                    for (int i=0; i<ui->dataTableWidget->rowCount(); i++)
                    {
                        if (ui->dataTableWidget->itemDelegateForRow(i))
                        {
                            mCellResults[i][j]=ui->dataTableWidget->item(i,j)->text();
                        }
                        else
                        {
                            mCellResults[i][j]=QTime::fromString(ui->dataTableWidget->item(i,j)->text(), "h:mm:ss");
                        }
                    }
                }
                break;
                case ctExpression:
                {
                    // Nothing, will be process later
                }
                break;
                default:
                {
                    throw "Unknown column type";
                }
                break;
            }
        }

        int lastNotCalculated=0;
        int curNotCalculated=0;

        do
        {
            curNotCalculated=0;

            for (int j=0; j<ui->dataTableWidget->columnCount(); j++)
            {
                if (
                    typeColumns.at(j).column->type()==ctExpression
                    &&
                    !mCellResults.at(0).at(j).isValid()
                   )
                {
                    curNotCalculated++;
                }
            }

            if (curNotCalculated==0)
            {
                break;
            }

            if (lastNotCalculated==curNotCalculated)
            {
                calculationError="Вычисление зациклилось";
                throw "Recursive error";
                break;
            }

            lastNotCalculated=curNotCalculated;

            for (int j=0; j<ui->dataTableWidget->columnCount(); j++)
            {
                if (
                    typeColumns.at(j).column->type()==ctExpression
                    &&
                    !mCellResults.at(0).at(j).isValid()
                   )
                {
                    for (int i=0; i<ui->dataTableWidget->rowCount(); i++)
                    {
                        try
                        {
                            if (ui->dataTableWidget->itemDelegateForRow(i))
                            {
                                mCellResults[i][j]=ui->dataTableWidget->item(i,j)->text();
                            }
                            else
                            {
                                mCellResults[i][j]=calculatePart(ui->dataTableWidget->item(i,j)->text(), this, this, i);
                            }
                        }
                        catch(...)
                        {
                            if (calculationError!="")
                            {
                                throw "Recursive throw";
                            }
                        }
                    }
                }
            }
        } while(true);
    }

//----------------------------------------------

    QVariantList resultRows;

    for (int i=0; i<mCellResults.length(); i++)
    {
        QVariantList resultCells;

        for (int j=0; j<mCellResults.at(i).length(); j++)
        {
            resultCells.append(mCellResults.at(i).at(j));
        }

        resultRows.append(QVariant(resultCells));
    }

    mCellResults.clear();
    calculationResult=resultRows;

    isWasCalculated=true;
    isInCalculation=false;

    return calculationResult;
}

void VariableExtendedListFrame::getWidgetList(QList<QWidget*> &aList)
{
    if (isAdmin || isEditable())
    {
        if (isAdmin)
        {
            aList.append(ui->nameEdit);
            aList.append(ui->varNameEdit);
        }

        if (ui->useCheckBox->isChecked())
        {
            aList.append(ui->dataTableWidget);
        }
    }
}

void VariableExtendedListFrame::on_expandButton_clicked()
{
    FullDialog dialog(this);
    dialog.setWindowTitle(ui->nameEdit->text());

    //------------------------------------------

    ui->mainVerticalLayout->removeWidget(ui->expandedWidget);

    ui->expandButton->setVisible(false);
    ui->lockButton->setVisible(false);

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
    ui->useCheckBox->setVisible(mIsTable);
    ui->titleLabel->setVisible(!mIsTable);

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
                    aItem->setText(((IntegerColumn*)typeColumns.at(i).column)->mPrefix+QString::number(((IntegerColumn*)typeColumns.at(i).column)->mDefaultValue, 'f', ((IntegerColumn*)typeColumns.at(i).column)->mDecimals)+((IntegerColumn*)typeColumns.at(i).column)->mPostfix);
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
                aItem->setText((((ListColumn*)typeColumns.at(i).column)->mDefaultValue));
            }
            break;
            case ctExtendedList:
            {
                aItem->setText((((ExtendedListColumn*)typeColumns.at(i).column)->mDefaultValue));
            }
            break;
            case ctExpression:
            {
                aItem->setText((((ExpressionColumn*)typeColumns.at(i).column)->mDefaultValue));
            }
            break;
            default:
            break;
        }

        ui->dataTableWidget->setItem(row, i, aItem);
    }

    ui->dataTableWidget->setItemDelegateForRow(row, 0);
}

void VariableExtendedListFrame::setItemsForMiddleRow(int row)
{
    QBrush aBrush(middleRowBackgroundColor);

    for (int i=0; i<ui->dataTableWidget->columnCount(); i++)
    {
        QTableWidgetItem *aItem=new QTableWidgetItem();

        QFont aFont;
        aFont.fromString(middleRowFontString);
        aItem->setFont(aFont);

        aItem->setTextAlignment(middleRowAlignment);
        aItem->setBackground(aBrush);
        aItem->setTextColor(middleRowTextColor);

        ui->dataTableWidget->setItem(row, i, aItem);
    }

    ui->dataTableWidget->setSpan(row, 0, 1, ui->dataTableWidget->columnCount());

    if (mLinkForMiddleRow=="")
    {
        ui->dataTableWidget->setItemDelegateForRow(row, new StringDelegate(this));
    }
    else
    {
        ui->dataTableWidget->setItemDelegateForRow(row, new ListDelegate(mLinkForMiddleRow, this));
    }
}

void VariableExtendedListFrame::insertRow(int row)
{
    ui->dataTableWidget->insertRow(row);

    for (int j=ui->dataTableWidget->rowCount()-1; j>row; j--)
    {
        ui->dataTableWidget->setItemDelegateForRow(j, ui->dataTableWidget->itemDelegateForRow(j-1));
    }

    setItemsForRow(row);
}

void VariableExtendedListFrame::insertMiddleRow(int row)
{
    ui->dataTableWidget->insertRow(row);

    for (int j=ui->dataTableWidget->rowCount()-1; j>row; j--)
    {
        ui->dataTableWidget->setItemDelegateForRow(j, ui->dataTableWidget->itemDelegateForRow(j-1));
    }

    setItemsForMiddleRow(row);
}

void VariableExtendedListFrame::on_addRowButton_clicked()
{
    insertRow(ui->dataTableWidget->rowCount());

    ui->dataTableWidget->scrollToBottom();
}

void VariableExtendedListFrame::dataTableInsertRowBefore()
{
    QInputDialog dialog(this);

    dialog.setWindowTitle("Вставка");
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(1);
    dialog.setIntMaximum(10000);
    dialog.setIntValue(1);
    dialog.setLabelText("Количество строк:");

    if (dialog.exec())
    {
        int row=ui->dataTableWidget->currentRow();

        for (int i=0; i<dialog.intValue(); i++)
        {
            insertRow(row);
            row++;
        }
    }
}

void VariableExtendedListFrame::dataTableInsertRowAfter()
{
    QInputDialog dialog(this);

    dialog.setWindowTitle("Вставка");
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(1);
    dialog.setIntMaximum(10000);
    dialog.setIntValue(1);
    dialog.setLabelText("Количество строк:");

    if (dialog.exec())
    {
        int row=ui->dataTableWidget->currentRow()+1;

        for (int i=0; i<dialog.intValue(); i++)
        {
            insertRow(row);
            row++;
        }
    }
}

void VariableExtendedListFrame::on_addMiddleRowButton_clicked()
{
    insertMiddleRow(ui->dataTableWidget->rowCount());

    ui->dataTableWidget->scrollToBottom();
}

void VariableExtendedListFrame::dataTableInsertMiddleRowBefore()
{
    QInputDialog dialog(this);

    dialog.setWindowTitle("Вставка");
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(1);
    dialog.setIntMaximum(10000);
    dialog.setIntValue(1);
    dialog.setLabelText("Количество строк:");

    if (dialog.exec())
    {
        int row=ui->dataTableWidget->currentRow();

        for (int i=0; i<dialog.intValue(); i++)
        {
            insertMiddleRow(row);
            row++;
        }
    }
}

void VariableExtendedListFrame::dataTableInsertMiddleRowAfter()
{
    QInputDialog dialog(this);

    dialog.setWindowTitle("Вставка");
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(1);
    dialog.setIntMaximum(10000);
    dialog.setIntValue(1);
    dialog.setLabelText("Количество строк:");

    if (dialog.exec())
    {
        int row=ui->dataTableWidget->currentRow()+1;

        for (int i=0; i<dialog.intValue(); i++)
        {
            insertMiddleRow(row);
            row++;
        }
    }
}

void VariableExtendedListFrame::dataTableCopyRows()
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
        int min=aRows.at(e);
        int minIndex=e;

        for (int i=e+1; i<aRows.length(); i++)
        {
            if (aRows.at(i)<min)
            {
                min=aRows.at(i);
                minIndex=i;
            }
        }

        aRows.swap(e, minIndex);
    }

    QString toClipBoard="";

    for (int i=0; i<aRows.length(); i++)
    {
        int realRow=aRows.at(i);
        QString aRow="";

        if (ui->dataTableWidget->itemDelegateForRow(realRow))
        {
            if (ui->dataTableWidget->columnCount()>0)
            {
                aRow="#### "+ui->dataTableWidget->item(realRow, 0)->text();
            }
        }
        else
        {
            for (int j=0; j<ui->dataTableWidget->columnCount(); j++)
            {
                if (aRow!="")
                {
                    aRow.append("\t");
                }

                if (ui->dataTableWidget->item(realRow, j)->data(Qt::CheckStateRole).isValid())
                {
                    if (ui->dataTableWidget->item(realRow, j)->checkState()==Qt::Checked)
                    {
                        aRow.append("1");
                    }
                    else
                    {
                        aRow.append("0");
                    }
                }
                else
                {
                    aRow.append(ui->dataTableWidget->item(realRow, j)->text());
                }
            }
        }

        if (aRow!="" && toClipBoard!="")
        {
            toClipBoard.append("\n");
        }

        toClipBoard.append(aRow);
    }

    QApplication::clipboard()->setText(toClipBoard);
}

void VariableExtendedListFrame::dataTablePasteBefore()
{
    QString aClipboard=QApplication::clipboard()->text().replace("\r","");

    if (aClipboard.endsWith("\n"))
    {
        aClipboard.remove(aClipboard.length()-1, 1);
    }

    if (aClipboard=="")
    {
        return;
    }

    QStringList rows=aClipboard.split("\n");

    int row=ui->dataTableWidget->currentRow();
    int origRow=row;

    for (int i=0; i<rows.length(); i++)
    {
        insertRow(row);
        row++;
    }

    QTableWidgetItem *lastItem=ui->dataTableWidget->currentItem();

    ui->dataTableWidget->setCurrentCell(origRow, 0);
    ui->dataTableWidget->pasteData();

    ui->dataTableWidget->setCurrentItem(lastItem);
    ui->dataTableWidget->scrollToItem(lastItem);
}

void VariableExtendedListFrame::dataTablePasteAfter()
{
    QString aClipboard=QApplication::clipboard()->text().replace("\r","");

    if (aClipboard.endsWith("\n"))
    {
        aClipboard.remove(aClipboard.length()-1, 1);
    }

    if (aClipboard=="")
    {
        return;
    }

    QStringList rows=aClipboard.split("\n");

    int row=ui->dataTableWidget->currentRow()+1;
    int origRow=row;

    for (int i=0; i<rows.length(); i++)
    {
        insertRow(row);
        row++;
    }

    QTableWidgetItem *lastItem=ui->dataTableWidget->currentItem();

    ui->dataTableWidget->setCurrentCell(origRow, 0);
    ui->dataTableWidget->pasteData();

    ui->dataTableWidget->setCurrentItem(lastItem);
    ui->dataTableWidget->scrollToItem(lastItem);
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

    for (int i=0; i<ui->dataTableWidget->columnCount(); i++)
    {
        if (
            typeColumns.at(i).column->type()==ctInteger
            &&
            ((IntegerColumn*)typeColumns.at(i).column)->mIsAutoInc
           )
        {
            int id=1;

            for (int j=0; j<ui->dataTableWidget->rowCount(); j++)
            {
                if (ui->dataTableWidget->itemDelegateForRow(j)==0)
                {
                    ui->dataTableWidget->item(j,i)->setText(QString::number(id));
                    id++;
                }
            }
        }
    }
}

void VariableExtendedListFrame::on_addFromAnotherButton_clicked()
{
    VariableExtendedListFrame *aFrame=0;

    QString aLink=mLinkForAnotherList;

    int index=aLink.indexOf(".");

    if (index>=0)
    {
        QString aSectionName=aLink.left(index);
        QString aVarName=aLink.mid(index+1);

        if (aSectionName=="Global")
        {
            for (int i=0; i<globalDialog->variables.length(); i++)
            {
                if (globalDialog->variables.at(i)->variableName()==aVarName)
                {
                    if (globalDialog->variables.at(i)->inherits("VariableExtendedListFrame"))
                    {
                        aFrame=(VariableExtendedListFrame*)globalDialog->variables[i];
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
                            if (aPage->variables.at(j)->inherits("VariableExtendedListFrame"))
                            {
                                aFrame=(VariableExtendedListFrame*)aPage->variables[j];
                            }

                            break;
                        }
                    }

                    for (int j=0; j<aPage->components.length(); j++)
                    {
                        if (aPage->components.at(j)->variableName()==aVarName)
                        {
                            if (aPage->components.at(j)->inherits("VariableExtendedListFrame"))
                            {
                                aFrame=(VariableExtendedListFrame*)aPage->components[j];
                            }

                            break;
                        }
                    }
                }
            }
        }
    }

    if (aFrame)
    {
        if (ui->dataTableWidget->columnCount()==0)
        {
            QMessageBox::information(this, "Вставка из расширенного списка", "Нет столбцов в исходном расширенном списке");
            return;
        }

        if (aFrame->ui->dataTableWidget->rowCount()==0 || aFrame->ui->dataTableWidget->columnCount()==0)
        {
            QMessageBox::information(this, "Вставка из расширенного списка", "Нет данных в связанной таблице");
            return;
        }

        InsertLinkTableDialog dialog(aFrame, this, this);
        dialog.exec();
    }
    else
    {
        if (mLinkForAnotherList=="")
        {
            QMessageBox::information(this, "Вставка из расширенного списка", "Отсуствует привязка к расширенному списку");
        }
        else
        {
            QMessageBox::information(this, "Вставка из расширенного списка", "Не найдена привязка к расширенному списку \""+mLinkForAnotherList+"\"");
        }
    }
}

void VariableExtendedListFrame::dataTableFont()
{
    QFontDialog dialog(ui->dataTableWidget->currentItem()->font(), this);

    if (dialog.exec())
    {
        QList<QTableWidgetSelectionRange> aRanges=ui->dataTableWidget->selectedRanges();

        for (int i=0; i<aRanges.length(); i++)
        {
            for (int j=aRanges.at(i).topRow(); j<=aRanges.at(i).bottomRow(); j++)
            {
                for (int k=aRanges.at(i).leftColumn(); k<=aRanges.at(i).rightColumn(); k++)
                {
                    ui->dataTableWidget->item(j, k)->setFont(dialog.selectedFont());
                }
            }
        }
    }
}

void VariableExtendedListFrame::dataTableBackgroundColor()
{
    QColorDialog dialog(ui->dataTableWidget->currentItem()->background().color(), this);

    if (dialog.exec())
    {
        QBrush aNewBrush(dialog.selectedColor());

        QList<QTableWidgetSelectionRange> aRanges=ui->dataTableWidget->selectedRanges();

        for (int i=0; i<aRanges.length(); i++)
        {
            for (int j=aRanges.at(i).topRow(); j<=aRanges.at(i).bottomRow(); j++)
            {
                for (int k=aRanges.at(i).leftColumn(); k<=aRanges.at(i).rightColumn(); k++)
                {
                    ui->dataTableWidget->item(j, k)->setBackground(aNewBrush);
                }
            }
        }
    }
}

void VariableExtendedListFrame::dataTableTextColor()
{
    QColorDialog dialog(ui->dataTableWidget->currentItem()->textColor(), this);

    if (dialog.exec())
    {
        QList<QTableWidgetSelectionRange> aRanges=ui->dataTableWidget->selectedRanges();

        for (int i=0; i<aRanges.length(); i++)
        {
            for (int j=aRanges.at(i).topRow(); j<=aRanges.at(i).bottomRow(); j++)
            {
                for (int k=aRanges.at(i).leftColumn(); k<=aRanges.at(i).rightColumn(); k++)
                {
                    ui->dataTableWidget->item(j, k)->setTextColor(dialog.selectedColor());
                }
            }
        }
    }
}

void VariableExtendedListFrame::tableAlignmentShow()
{
    mCellAlignmentWidget->show();
}

void VariableExtendedListFrame::tableAlignmentHide()
{
    mCellAlignmentWidget->hide();
}

void VariableExtendedListFrame::setItemsAlignment(int aAlignment)
{
    QList<QTableWidgetSelectionRange> aRanges=ui->dataTableWidget->selectedRanges();

    for (int i=0; i<aRanges.length(); i++)
    {
        for (int j=aRanges.at(i).topRow(); j<=aRanges.at(i).bottomRow(); j++)
        {
            for (int k=aRanges.at(i).leftColumn(); k<=aRanges.at(i).rightColumn(); k++)
            {
                ui->dataTableWidget->item(j, k)->setTextAlignment(aAlignment);
            }
        }
    }
}

void VariableExtendedListFrame::tableCellAlignTopLeft()
{
    setItemsAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void VariableExtendedListFrame::tableCellAlignTop()
{
    setItemsAlignment(Qt::AlignTop | Qt::AlignHCenter);
}

void VariableExtendedListFrame::tableCellAlignTopRight()
{
    setItemsAlignment(Qt::AlignTop | Qt::AlignRight);
}

void VariableExtendedListFrame::tableCellAlignLeft()
{
    setItemsAlignment(Qt::AlignVCenter | Qt::AlignLeft);
}

void VariableExtendedListFrame::tableCellAlignCenter()
{
    setItemsAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
}

void VariableExtendedListFrame::tableCellAlignRight()
{
    setItemsAlignment(Qt::AlignVCenter | Qt::AlignRight);
}

void VariableExtendedListFrame::tableCellAlignBottomLeft()
{
    setItemsAlignment(Qt::AlignBottom | Qt::AlignLeft);
}

void VariableExtendedListFrame::tableCellAlignBottom()
{
    setItemsAlignment(Qt::AlignBottom | Qt::AlignHCenter);
}

void VariableExtendedListFrame::tableCellAlignBottomRight()
{
    setItemsAlignment(Qt::AlignBottom | Qt::AlignRight);
}

void VariableExtendedListFrame::on_dataTableWidget_customContextMenuRequested(const QPoint &pos)
{
    bool itemSelected=ui->dataTableWidget->selectedItems().length()>0;
    QString aClipboard=QApplication::clipboard()->text().replace("\r","");

    QMenu *contextMenu=new QMenu;

    contextMenu->addAction("Вставить строки перед текущей",               this, SLOT(dataTableInsertRowBefore()))->setEnabled(itemSelected);
    contextMenu->addAction("Вставить строки после текущей",               this, SLOT(dataTableInsertRowAfter()))->setEnabled(itemSelected);
    contextMenu->addAction("Вставить промежуточные строки перед текущей", this, SLOT(dataTableInsertMiddleRowBefore()))->setEnabled(itemSelected);
    contextMenu->addAction("Вставить промежуточные строки после текущей", this, SLOT(dataTableInsertMiddleRowAfter()))->setEnabled(itemSelected);
    contextMenu->addSeparator();
    contextMenu->addAction("Копировать строки в буфер",                   this, SLOT(dataTableCopyRows()))->setEnabled(itemSelected);
    contextMenu->addAction("Вставить строки из буфера перед текущей",     this, SLOT(dataTablePasteBefore()))->setEnabled(itemSelected && aClipboard!="");
    contextMenu->addAction("Вставить строки из буфера после текущей",     this, SLOT(dataTablePasteAfter()))->setEnabled(itemSelected && aClipboard!="");
    contextMenu->addSeparator();
    contextMenu->addAction("Удалить строку(и)",                           this, SLOT(on_deleteRowButton_clicked()))->setEnabled(itemSelected);
    contextMenu->addSeparator();
    contextMenu->addAction("Шрифт",                                       this, SLOT(dataTableFont()))->setEnabled(itemSelected);
    contextMenu->addAction("Цвет ячейки",                                 this, SLOT(dataTableBackgroundColor()))->setEnabled(itemSelected);
    contextMenu->addAction("Цвет текста",                                 this, SLOT(dataTableTextColor()))->setEnabled(itemSelected);
    contextMenu->addSeparator();

    QMenu *cellAlignMenu=contextMenu->addMenu("Положение в ячейке");

    if (itemSelected)
    {
        int aTextAlignment=ui->dataTableWidget->currentItem()->textAlignment();

        mCellAlignmentWidget->ui->topLeftButton    ->setIcon(aTextAlignment==33  ? QIcon(":/images/CellTopLeftSelected.png")     : QIcon(":/images/CellTopLeft.png"));
        mCellAlignmentWidget->ui->topButton        ->setIcon(aTextAlignment==36  ? QIcon(":/images/CellTopSelected.png")         : QIcon(":/images/CellTop.png"));
        mCellAlignmentWidget->ui->topRightButton   ->setIcon(aTextAlignment==34  ? QIcon(":/images/CellTopRightSelected.png")    : QIcon(":/images/CellTopRight.png"));
        mCellAlignmentWidget->ui->leftButton       ->setIcon(aTextAlignment==129 ? QIcon(":/images/CellLeftSelected.png")        : QIcon(":/images/CellLeft.png"));
        mCellAlignmentWidget->ui->centerButton     ->setIcon(aTextAlignment==132 ? QIcon(":/images/CellCenterSelected.png")      : QIcon(":/images/CellCenter.png"));
        mCellAlignmentWidget->ui->rightButton      ->setIcon(aTextAlignment==130 ? QIcon(":/images/CellRightSelected.png")       : QIcon(":/images/CellRight.png"));
        mCellAlignmentWidget->ui->bottomLeftButton ->setIcon(aTextAlignment==65  ? QIcon(":/images/CellBottomLeftSelected.png")  : QIcon(":/images/CellBottomLeft.png"));
        mCellAlignmentWidget->ui->bottomButton     ->setIcon(aTextAlignment==68  ? QIcon(":/images/CellBottomSelected.png")      : QIcon(":/images/CellBottom.png"));
        mCellAlignmentWidget->ui->bottomRightButton->setIcon(aTextAlignment==66  ? QIcon(":/images/CellBottomRightSelected.png") : QIcon(":/images/CellBottomRight.png"));

        setGeometryInDesktop(mCellAlignmentWidget,
                             cursor().pos().x()+contextMenu->sizeHint().width()-10,
                             cursor().pos().y()+contextMenu->sizeHint().height()-15,
                             mCellAlignmentWidget->width(),
                             mCellAlignmentWidget->height());

        connect(cellAlignMenu, SIGNAL(aboutToShow()), this, SLOT(tableAlignmentShow()));
        connect(cellAlignMenu, SIGNAL(aboutToHide()), this, SLOT(tableAlignmentHide()));
    }
    else
    {
        cellAlignMenu->setEnabled(false);
    }

    setGeometryInDesktop(contextMenu,
                         cursor().pos().x(),
                         cursor().pos().y(),
                         contextMenu->sizeHint().width(),
                         contextMenu->sizeHint().height());

    contextMenu->show();
}

void VariableExtendedListFrame::dataTableHeaderMove(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    disconnect(ui->dataTableWidget->verticalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(dataTableHeaderMove(int,int,int)));

    ui->dataTableWidget->verticalHeader()->moveSection(newVisualIndex, oldVisualIndex);

    QTableWidgetItem *takenItem;
    int step=oldVisualIndex<newVisualIndex? 1 : -1;

    for (int j=0; j<ui->dataTableWidget->columnCount(); j++)
    {
        takenItem=ui->dataTableWidget->takeItem(oldVisualIndex, j);

        for (int i=oldVisualIndex; i!=newVisualIndex; i+=step)
        {
            ui->dataTableWidget->setItem(i, j, ui->dataTableWidget->takeItem(i+step, j));
        }

        ui->dataTableWidget->setItem(newVisualIndex, j, takenItem);
    }

    QAbstractItemDelegate *takenDelegate=ui->dataTableWidget->itemDelegateForRow(oldVisualIndex);

    for (int i=oldVisualIndex; i!=newVisualIndex; i+=step)
    {
        ui->dataTableWidget->setItemDelegateForRow(i, ui->dataTableWidget->itemDelegateForRow(i+step));
    }

    ui->dataTableWidget->setItemDelegateForRow(newVisualIndex, takenDelegate);

    int takenColSpan=ui->dataTableWidget->columnSpan(oldVisualIndex, 0);

    for (int i=oldVisualIndex; i!=newVisualIndex; i+=step)
    {
        ui->dataTableWidget->setSpan(i, 0, 1, ui->dataTableWidget->columnSpan(i+step, 0));
    }

    ui->dataTableWidget->setSpan(newVisualIndex, 0, 1, takenColSpan);

    for (int j=0; j<typeColumns.length(); j++)
    {
        if (
            typeColumns.at(j).column->type()==ctInteger
            &&
            ((IntegerColumn*)typeColumns.at(j).column)->mIsAutoInc
           )
        {
            int id=1;

            for (int i=0; i<ui->dataTableWidget->rowCount(); i++)
            {
                if (ui->dataTableWidget->itemDelegateForRow(i)==0)
                {
                    ui->dataTableWidget->item(i, j)->setText(QString::number(id));

                    id++;
                }
            }
        }
    }

    connect(ui->dataTableWidget->verticalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(dataTableHeaderMove(int,int,int)));
}
