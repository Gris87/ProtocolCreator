#include "src/other/global.h"

VariableExtendedListFrame::VariableExtendedListFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::VariableExtendedListFrame)
{
    ui->setupUi(this);

    mIsTable=false;
    needUpdateHeight=false;

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

    init();
}

VariableExtendedListFrame::~VariableExtendedListFrame()
{
    for (int i=0; i<typeColumns.length(); i++)
    {
        delete typeColumns.at(i).column;
    }

    delete ui;
}

void VariableExtendedListFrame::init()
{
    ui->nameEdit->setText("����������� ������");
    ui->varNameEdit->setText("ExtendedList");

    ui->useCheckBox->setChecked(true);

    ui->addFromAnotherButton->setEnabled(false);
    ui->copyFromAnotherTableButton->setEnabled(false);

    ui->addFromAnotherButton->setVisible(false);
    ui->copyFromAnotherTableButton->setVisible(false);

    mTableAlignment=Qt::AlignCenter;
    mTableOffset=0;
    mLinkForMiddleRow="";
    mLinkForAnotherList="";
    mLinkForCopyingAnotherList="";
    mCopyColumnCount=1;
    mCopyMiddleRow=true;
    mCopyRules=crAll;
    mRulesText.clear();
    mRulesColumn=0;

    headerCells.clear();
    headerColumnWidths.clear();
    cloneHeader=true;

    for (int i=0; i<typeColumns.length(); i++)
    {
        delete typeColumns.at(i).column;
    }

    typeColumns.clear();
    typeColumnWidths.clear();

    ui->dataTableWidget->clearContents();
    ui->dataTableWidget->setRowCount(0);
    ui->dataTableWidget->setColumnCount(0);

    middleRowFontString="Times New Roman,12,-1,5,50,0,0,0,0,0";
    middleRowAlignment=Qt::AlignCenter;
    middleRowBackgroundColor=QColor(255, 255, 255);
    middleRowTextColor=QColor(0, 0, 0);
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

    if (ui->nameEdit->text()!="����������� ������")
    {
        aStream << QString("Name");
        aStream << ui->nameEdit->text();
    }

    if (ui->varNameEdit->text()!="ExtendedList")
    {
        aStream << QString("VarName");
        aStream << ui->varNameEdit->text();
    }

    if (!ui->useCheckBox->isChecked())
    {
        bool aUsed=false;

        aStream << QString("Used");
        aStream << aUsed;
    }

    if (!isEditable())
    {
        bool aEdit=false;

        aStream << QString("Edit");
        aStream << aEdit;
    }

    if (!ui->userWidget->isEnabled())
    {
        bool aLock=true;

        aStream << QString("Lock");
        aStream << aLock;
    }

    if (mTableAlignment!=Qt::AlignCenter)
    {
        int intTableAlignment=(int)mTableAlignment;

        aStream << QString("TableAlignment");
        aStream << intTableAlignment;
    }

    if (mTableOffset!=0)
    {
        aStream << QString("TableOffset");
        aStream << mTableOffset;
    }

    if (mLinkForMiddleRow!="")
    {
        aStream << QString("LinkForMiddleRow");
        aStream << mLinkForMiddleRow;
    }

    if (mLinkForAnotherList!="")
    {
        aStream << QString("LinkForAnotherList");
        aStream << mLinkForAnotherList;
    }

    if (mLinkForCopyingAnotherList!="")
    {
        aStream << QString("LinkForCopyingAnotherList");
        aStream << mLinkForCopyingAnotherList;

        if (mCopyColumnCount>1)
        {
            aStream << QString("CopyColumnCount");
            aStream << mCopyColumnCount;
        }

        if (!mCopyMiddleRow)
        {
            aStream << QString("CopyMiddleRow");
            aStream << mCopyMiddleRow;
        }

        if (mCopyRules!=crAll)
        {
            quint8 aCopyRules=(quint8)mCopyRules;
            aStream << QString("CopyRules");
            aStream << aCopyRules;
        }

        int rulesCount=mRulesText.length();

        if (rulesCount>0)
        {
            aStream << QString("RulesText");
            aStream << rulesCount;

            for (int i=0; i<rulesCount; ++i)
            {
                aStream << mRulesText.at(i);
            }
        }

        if (mRulesColumn>0)
        {
            aStream << QString("RulesColumn");
            aStream << mRulesColumn;
        }
    }

    int headerRowCount=headerCells.length();
    int headerColCount=headerRowCount==0? 0 : headerCells.at(0).length();

    if (headerColCount>0)
    {
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

                if (aCell->spanX>1)
                {
                    aStream << QString("SpanX");
                    aStream << aCell->spanX;
                }

                if (aCell->spanY>1)
                {
                    aStream << QString("SpanY");
                    aStream << aCell->spanY;
                }

                if (aCell->fontString!="Times New Roman,12,-1,5,50,0,0,0,0,0")
                {
                    aStream << QString("Font");
                    aStream << aCell->fontString;
                }

                if (aCell->text!="")
                {
                    aStream << QString("Text");
                    aStream << aCell->text;
                }

                if (aCell->alignment!=Qt::AlignCenter)
                {
                    aStream << QString("Alignment");
                    aStream << aCell->alignment;
                }

                if (
                    aCell->backgroundColorR!=255
                    ||
                    aCell->backgroundColorG!=255
                    ||
                    aCell->backgroundColorB!=255
                   )
                {
                    aStream << QString("Background");
                    aStream << aCell->backgroundColorR;
                    aStream << aCell->backgroundColorG;
                    aStream << aCell->backgroundColorB;
                }

                if (
                    aCell->textColorR!=0
                    ||
                    aCell->textColorG!=0
                    ||
                    aCell->textColorB!=0
                   )
                {
                    aStream << QString("TextColor");
                    aStream << aCell->textColorR;
                    aStream << aCell->textColorG;
                    aStream << aCell->textColorB;
                }

                aStream << QString("CellEnd");
            }
        }

        if (!cloneHeader)
        {
            aStream << QString("CloneToPage");
            aStream << cloneHeader;
        }

        aStream << QString("HeaderEnd");
    }

    int typesCount=typeColumns.length();

    if (typesCount>0)
    {
        aStream << QString("ColTypes");

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

            if (aColumn->name!="")
            {
                aStream << QString("Name");
                aStream << aColumn->name;
            }

            if (!aColumn->visible)
            {
                aStream << QString("Visible");
                aStream << aColumn->visible;
            }

            aStream << QString("Column");
            aColumn->column->saveToStream(aStream);

            if (aColumn->leftMargin!=0.19)
            {
                aStream << QString("LeftMargin");
                aStream << aColumn->leftMargin;
            }

            if (aColumn->rightMargin!=0.19)
            {
                aStream << QString("RightMargin");
                aStream << aColumn->rightMargin;
            }

            if (aColumn->topMargin!=0)
            {
                aStream << QString("TopMargin");
                aStream << aColumn->topMargin;
            }

            if (aColumn->bottomMargin!=0)
            {
                aStream << QString("BottomMargin");
                aStream << aColumn->bottomMargin;
            }

            if (aColumn->fontString!="Times New Roman,12,-1,5,50,0,0,0,0,0")
            {
                aStream << QString("Font");
                aStream << aColumn->fontString;
            }

            if (aColumn->alignment!=(Qt::AlignVCenter | Qt::AlignLeft))
            {
                aStream << QString("Alignment");
                aStream << aColumn->alignment;
            }

            if (
                aColumn->backgroundColorR!=255
                ||
                aColumn->backgroundColorG!=255
                ||
                aColumn->backgroundColorB!=255
               )
            {
                aStream << QString("Background");
                aStream << aColumn->backgroundColorR;
                aStream << aColumn->backgroundColorG;
                aStream << aColumn->backgroundColorB;
            }

            if (
                aColumn->textColorR!=0
                ||
                aColumn->textColorG!=0
                ||
                aColumn->textColorB!=0
               )
            {
                aStream << QString("TextColor");
                aStream << aColumn->textColorR;
                aStream << aColumn->textColorG;
                aStream << aColumn->textColorB;
            }

            int condCount=aColumn->conditions.length();

            if (condCount>0)
            {
                aStream << QString("Conditions");
                aStream << condCount;

                for (int j=0; j<condCount; j++)
                {
                    SConditionFormat *aFormat=&aColumn->conditions[j];

                    if (aFormat->fontString!="Times New Roman,12,-1,5,50,0,0,0,0,0")
                    {
                        aStream << QString("Font");
                        aStream << aFormat->fontString;
                    }

                    if (aFormat->alignment!=(Qt::AlignVCenter | Qt::AlignLeft))
                    {
                        aStream << QString("Alignment");
                        aStream << aFormat->alignment;
                    }

                    if (
                        aFormat->backgroundColorR!=255
                        ||
                        aFormat->backgroundColorG!=255
                        ||
                        aFormat->backgroundColorB!=255
                       )
                    {
                        aStream << QString("Background");
                        aStream << aFormat->backgroundColorR;
                        aStream << aFormat->backgroundColorG;
                        aStream << aFormat->backgroundColorB;
                    }

                    if (
                        aFormat->textColorR!=0
                        ||
                        aFormat->textColorG!=0
                        ||
                        aFormat->textColorB!=0
                       )
                    {
                        aStream << QString("TextColor");
                        aStream << aFormat->textColorR;
                        aStream << aFormat->textColorG;
                        aStream << aFormat->textColorB;
                    }

                    if (aFormat->condition!="")
                    {
                        aStream << QString("Condition");
                        aStream << aFormat->condition;
                    }

                    if (!aFormat->needWarning)
                    {
                        aStream << QString("Warning");
                        aStream << aFormat->needWarning;
                    }

                    aStream << QString("FormatEnd");
                }
            }

            aStream << QString("TypeEnd");
        }

        aStream << QString("ColTypesEnd");
    }

    if (middleRowFontString!="Times New Roman,12,-1,5,50,0,0,0,0,0")
    {
        aStream << QString("MiddleFont");
        aStream << middleRowFontString;
    }

    if (middleRowAlignment!=Qt::AlignCenter)
    {
        aStream << QString("MiddleAlignment");
        aStream << middleRowAlignment;
    }

    if (middleRowBackgroundColor!=QColor(255, 255, 255))
    {
        aStream << QString("MiddleBackground");
        aStream << (quint8)middleRowBackgroundColor.red();
        aStream << (quint8)middleRowBackgroundColor.green();
        aStream << (quint8)middleRowBackgroundColor.blue();
    }

    if (middleRowTextColor!=QColor(0, 0, 0))
    {
        aStream << QString("MiddleTextColor");
        aStream << (quint8)middleRowTextColor.red();
        aStream << (quint8)middleRowTextColor.green();
        aStream << (quint8)middleRowTextColor.blue();
    }

    if (typesCount>0)
    {
        aStream << QString("Data");

        aStream << QString("ColumnWidth");

        for (int i=0; i<typesCount; i++)
        {
            aStream << ui->dataTableWidget->columnWidth(i);
        }

        int rowsCount=ui->dataTableWidget->rowCount();

        if (rowsCount>0)
        {
            aStream << QString("Rows");

            aStream << rowsCount;

            QTableWidgetItem *aItem;
            QColor aColor;
            QFont defaultFont("Times New Roman", 12);

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

                    if (aItem->font()!=defaultFont)
                    {
                        if (i>0 && aItem->font()==ui->dataTableWidget->item(i-1, j)->font())
                        {
                            aStream << QString("SameFont");
                        }
                        else
                        {
                            aStream << QString("Font");
                            aStream << aItem->font().toString();
                        }
                    }

                    if (aItem->textAlignment()!=(Qt::AlignVCenter | Qt::AlignLeft))
                    {
                        aStream << QString("Alignment");
                        aStream << aItem->textAlignment();
                    }

                    aColor=aItem->background().color();

                    if (aColor!=QColor(255, 255, 255))
                    {
                        aStream << QString("Background");
                        aStream << (quint8)aColor.red();
                        aStream << (quint8)aColor.green();
                        aStream << (quint8)aColor.blue();
                    }

                    aColor=aItem->textColor();

                    if (aColor!=QColor(0, 0, 0))
                    {
                        aStream << QString("TextColor");
                        aStream << (quint8)aColor.red();
                        aStream << (quint8)aColor.green();
                        aStream << (quint8)aColor.blue();
                    }

                    if (aItem->text()!="")
                    {
                        if (i>0 && aItem->text()==ui->dataTableWidget->item(i-1, j)->text())
                        {
                            aStream << QString("SameText");
                        }
                        else
                        {
                            aStream << QString("Text");
                            aStream << aItem->text();
                        }
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
        }

        aStream << QString("DataEnd");
    }

    aStream << QString("VarEnd");
}

void VariableExtendedListFrame::loadFromStream(QDataStream &aStream)
{
    init();

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
            ui->addFromAnotherButton->setEnabled(mLinkForAnotherList!="");

            ui->addFromAnotherButton->setVisible(ui->addFromAnotherButton->isEnabled() || ui->copyFromAnotherTableButton->isEnabled());
            ui->copyFromAnotherTableButton->setVisible(ui->addFromAnotherButton->isVisible());
        }
        else
        if (aMagicWord=="LinkForCopyingAnotherList")
        {
            aStream >> mLinkForCopyingAnotherList;
            ui->copyFromAnotherTableButton->setEnabled(mLinkForCopyingAnotherList!="");

            ui->addFromAnotherButton->setVisible(ui->addFromAnotherButton->isEnabled() || ui->copyFromAnotherTableButton->isEnabled());
            ui->copyFromAnotherTableButton->setVisible(ui->addFromAnotherButton->isVisible());
        }
        else
        if (aMagicWord=="CopyColumnCount")
        {
            aStream >> mCopyColumnCount;
        }
        else
        if (aMagicWord=="CopyMiddleRow")
        {
            aStream >> mCopyMiddleRow;
        }
        else
        if (aMagicWord=="CopyRules")
        {
            quint8 aCopyRules;
            aStream >> aCopyRules;
            mCopyRules=(ECopyRules)aCopyRules;
        }
        else
        if (aMagicWord=="RulesText")
        {
            int rulesCount;
            aStream >> rulesCount;

            for (int i=0; i<rulesCount; ++i)
            {
                QString st;
                aStream >> st;

                mRulesText.append(st);
            }
        }
        else
        if (aMagicWord=="RulesColumn")
        {
            aStream >> mRulesColumn;
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

                            aCell.spanX=1;
                            aCell.spanY=1;
                            aCell.fontString="Times New Roman,12,-1,5,50,0,0,0,0,0";
                            aCell.text="";
                            aCell.alignment=Qt::AlignCenter;
                            aCell.backgroundColorR=255;
                            aCell.backgroundColorG=255;
                            aCell.backgroundColorB=255;
                            aCell.textColorR=0;
                            aCell.textColorG=0;
                            aCell.textColorB=0;

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
                if (aMagicWord=="CloneToPage")
                {
                    aStream >> cloneHeader;
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

            for (int i=0; i<aColumnCount; i++)
            {
                ui->dataTableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(""));
            }

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

                        aColumn.name="";
                        aColumn.visible=true;
                        aColumn.leftMargin=0.19;
                        aColumn.rightMargin=0.19;
                        aColumn.topMargin=0;
                        aColumn.bottomMargin=0;
                        aColumn.fontString="Times New Roman,12,-1,5,50,0,0,0,0,0";
                        aColumn.alignment=Qt::AlignVCenter | Qt::AlignLeft;
                        aColumn.backgroundColorR=255;
                        aColumn.backgroundColorG=255;
                        aColumn.backgroundColorB=255;
                        aColumn.textColorR=0;
                        aColumn.textColorG=0;
                        aColumn.textColorB=0;

                        while (!aStream.atEnd())
                        {
                            aStream >> aMagicWord;

                            if (aMagicWord=="Name")
                            {
                                aStream >> aColumn.name;
                                ui->dataTableWidget->horizontalHeaderItem(i)->setText(aColumn.name);
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

                                switch (aColumn.column->type())
                                {
                                    case ctInteger:
                                    {
                                        DoubleDelegate *delegate=new DoubleDelegate(this);

                                        delegate->mDecimals=((IntegerColumn*)aColumn.column)->mDecimals;

                                        if (
                                            !((IntegerColumn*)aColumn.column)->mIsAutoInc
                                            &&
                                            !((IntegerColumn*)aColumn.column)->mSplitRows
                                           )
                                        {
                                            delegate->mPrefix=((IntegerColumn*)aColumn.column)->mPrefix;
                                            delegate->mPostfix=((IntegerColumn*)aColumn.column)->mPostfix;
                                        }

                                        ui->dataTableWidget->setItemDelegateForColumn(typeColumns.length(), delegate);
                                    }
                                    break;
                                    case ctDate:
                                    {
                                        ui->dataTableWidget->setItemDelegateForColumn(typeColumns.length(), new DateDelegate(this));
                                    }
                                    break;
                                    case ctTime:
                                    {
                                        ui->dataTableWidget->setItemDelegateForColumn(typeColumns.length(), new TimeDelegate(this));
                                    }
                                    break;
                                    case ctList:
                                    {
                                        ui->dataTableWidget->setItemDelegateForColumn(typeColumns.length(), new ListDelegate(
                                                                                                                             ((ListColumn*)aColumn.column)->mLinkComponent,
                                                                                                                             this
                                                                                                                            ));
                                    }
                                    break;
                                    case ctExtendedList:
                                    {
                                        ui->dataTableWidget->setItemDelegateForColumn(typeColumns.length(), new ListDelegate(
                                                                                                                             ((ExtendedListColumn*)aColumn.column)->mLinkComponent,
                                                                                                                             this
                                                                                                                            ));
                                    }
                                    break;
                                    default:
                                    {
                                        ui->dataTableWidget->setItemDelegateForColumn(typeColumns.length(), new StringDelegate(this));
                                    }
                                    break;
                                }
                            }
                            else
                            if (aMagicWord=="LeftMargin")
                            {
                                aStream >> aColumn.leftMargin;
                            }
                            else
                            if (aMagicWord=="RightMargin")
                            {
                                aStream >> aColumn.rightMargin;
                            }
                            else
                            if (aMagicWord=="TopMargin")
                            {
                                aStream >> aColumn.topMargin;
                            }
                            else
                            if (aMagicWord=="BottomMargin")
                            {
                                aStream >> aColumn.bottomMargin;
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
                            if (aMagicWord=="Conditions")
                            {
                                int aCondCount;
                                aStream >> aCondCount;

                                for (int j=0; j<aCondCount; j++)
                                {
                                    SConditionFormat aFormat;

                                    aFormat.fontString="Times New Roman,12,-1,5,50,0,0,0,0,0";
                                    aFormat.alignment=Qt::AlignVCenter | Qt::AlignLeft;
                                    aFormat.backgroundColorR=255;
                                    aFormat.backgroundColorG=255;
                                    aFormat.backgroundColorB=255;
                                    aFormat.textColorR=0;
                                    aFormat.textColorG=0;
                                    aFormat.textColorB=0;
                                    aFormat.condition="";
                                    aFormat.needWarning=true;

                                    while (!aStream.atEnd())
                                    {
                                        aStream >> aMagicWord;

                                        if (aMagicWord=="Font")
                                        {
                                            aStream >> aFormat.fontString;
                                        }
                                        else
                                        if (aMagicWord=="Alignment")
                                        {
                                            aStream >> aFormat.alignment;
                                        }
                                        else
                                        if (aMagicWord=="Background")
                                        {
                                            aStream >> aFormat.backgroundColorR;
                                            aStream >> aFormat.backgroundColorG;
                                            aStream >> aFormat.backgroundColorB;
                                        }
                                        else
                                        if (aMagicWord=="TextColor")
                                        {
                                            aStream >> aFormat.textColorR;
                                            aStream >> aFormat.textColorG;
                                            aStream >> aFormat.textColorB;
                                        }
                                        else
                                        if (aMagicWord=="Condition")
                                        {
                                            aStream >> aFormat.condition;
                                        }
                                        else
                                        if (aMagicWord=="Warning")
                                        {
                                            aStream >> aFormat.needWarning;
                                        }
                                        else
                                        if (aMagicWord=="FormatEnd")
                                        {
                                            break;
                                        }
                                    }

                                    aColumn.conditions.append(aFormat);
                                }

                                if (aCondCount>0)
                                {
                                    QFont aFont=ui->dataTableWidget->horizontalHeaderItem(i)->font();
                                    aFont.setUnderline(true);
                                    ui->dataTableWidget->horizontalHeaderItem(i)->setFont(aFont);
                                }
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
                    QFont defaultFont("Times New Roman", 12);

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

                            aItem->setFont(defaultFont);
                            aItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                            aItem->setBackground(QBrush(QColor(255, 255, 255)));
                            aItem->setTextColor(QColor(0, 0, 0));

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
                                if (aMagicWord=="SameFont")
                                {
                                    aItem->setFont(ui->dataTableWidget->item(i-1, j)->font());
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
                                if (aMagicWord=="SameText")
                                {
                                    aItem->setText(ui->dataTableWidget->item(i-1, j)->text());
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

            needUpdateHeight=true;
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

    ui->controlWidget->setVisible(isAdmin || isEditable());

    if (ui->controlWidget->isVisible())
    {
        ui->dataTableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed | QAbstractItemView::AnyKeyPressed);
    }
    else
    {
        ui->dataTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

void VariableExtendedListFrame::componentShown()
{
    if (needUpdateHeight && ui->userWidget->isVisible())
    {
        needUpdateHeight=false;

        ui->dataTableWidget->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    }
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

void VariableExtendedListFrame::on_varNameEdit_textChanged(const QString &aName)
{
    ui->useCheckBox->setToolTip(aName);
    ui->titleLabel->setToolTip(aName);
}

void VariableExtendedListFrame::on_useCheckBox_toggled(bool checked)
{
    ui->userWidget->setVisible(ui->userWidget->isEnabled() && checked);
    componentShown();
}

void VariableExtendedListFrame::on_lockButton_clicked()
{
    ui->userWidget->setEnabled(!ui->userWidget->isEnabled());

    updateLock();
    componentShown();
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

    ui->userWidget->setVisible(ui->userWidget->isEnabled() && ui->useCheckBox->isChecked());
}

void VariableExtendedListFrame::on_editButton_clicked()
{
    ui->editButton->setFlat(!ui->editButton->isFlat());
}

bool VariableExtendedListFrame::isEditable()
{
    return !ui->editButton->isFlat();
}

void VariableExtendedListFrame::checkLink(QStringList &aErrorList, QString aLink, bool needColumnIndex)
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
                            (
                             !needColumnIndex
                             ||
                             aColumnIndex!=""
                            )
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
                                    (
                                     !needColumnIndex
                                     ||
                                     aColumnIndex!=""
                                    )
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
                                    (
                                     !needColumnIndex
                                     ||
                                     aColumnIndex!=""
                                    )
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
                if (needColumnIndex)
                {
                    bool ok;
                    int aColumnID=aColumnIndex.toInt(&ok)-1;

                    if (!ok || aColumnID<0 || aColumnID>=aExtFrame->typeColumns.length())
                    {
                        if (ok)
                        {
                            if (aSectionName=="Global")
                            {
                                aErrorList.append("Hint: ������ ������������ � ���������� ���������� \""+aVarName+"\"");
                            }
                            else
                            {
                                aErrorList.append("Hint: ������ ������������ � ���������� \""+aVarName+"\" � ������� \""+aSectionName+"\"");
                            }
                        }
                        else
                        {
                            if (aSectionName=="Global")
                            {
                                aErrorList.append("Hint: ������ ������� �� ����� � ���������� ���������� \""+aVarName+"\"");
                            }
                            else
                            {
                                aErrorList.append("Hint: ������ ������� �� ����� � ���������� \""+aVarName+"\" � ������� \""+aSectionName+"\"");
                            }
                        }
                    }
                }
            }
            else
            if (aFrame==0)
            {
                if (aSectionName=="Global")
                {
                    aErrorList.append("Hint: �� ������� ���������� ���������� \""+aVarName+"\"");
                }
                else
                {
                    aErrorList.append("Hint: �� ������� ���������� \""+aVarName+"\" � ������� \""+aSectionName+"\"");
                }
            }
        }
        else
        {
            aErrorList.append("Hint: �� ������� ����� ��� ����������� ����� �� �������");
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
            aErrorList.append("Error: ���������� ������� �������� � ������� �� ��������� � ����������� �������� � ���������");
        }
    }

    checkLink(aErrorList, mLinkForMiddleRow, true);
    checkLink(aErrorList, mLinkForAnotherList, false);
    checkLink(aErrorList, mLinkForCopyingAnotherList, false);

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

        checkLink(aErrorList, aLink, true);
    }
}

QVariant VariableExtendedListFrame::calculate(QStringList *aErrorList)
{
    if (isWasCalculated)
    {
        return calculationResult;
    }

    PageComponent::calculate(aErrorList);

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

                    if (
                        !((IntegerColumn*)typeColumns.at(j).column)->mIsAutoInc
                        &&
                        !((IntegerColumn*)typeColumns.at(j).column)->mSplitRows
                       )
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
                calculationError="���������� �����������";
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
                                mCellResults[i][j]=calculatePart(ui->dataTableWidget->item(i,j)->text(), aErrorList, this, this, i);
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
// Conditional format

    for (int j=0; j<ui->dataTableWidget->columnCount(); j++)
    {
        for (int i=0; i<ui->dataTableWidget->rowCount(); i++)
        {
            for (int k=0; k<typeColumns.at(j).conditions.length(); k++)
            {
                SConditionFormat *aFormat=&typeColumns[j].conditions[k];

                QVariant aConditionResult=calculatePart(aFormat->condition, aErrorList, this, this, i);

                if (aConditionResult.type()==QVariant::Bool)
                {
                    if (aConditionResult.toBool())
                    {
                        QTableWidgetItem *aItem=ui->dataTableWidget->item(i, j);

                        QFont aFont;
                        aFont.fromString(aFormat->fontString);
                        aItem->setFont(aFont);

                        aItem->setTextAlignment(aFormat->alignment);
                        aItem->setBackground(QBrush(QColor(aFormat->backgroundColorR, aFormat->backgroundColorG, aFormat->backgroundColorB)));
                        aItem->setTextColor(QColor(aFormat->textColorR, aFormat->textColorG, aFormat->textColorB));

                        if (aFormat->needWarning)
                        {
                            aErrorList->append("Hint: �������� �������������� ��� ������ � "+QString::number(i+1)+"-� ������ � � ������� \""+typeColumns.at(j).name+"\"");
                        }

                        break;
                    }
                }
                else
                {
                    if (aErrorList)
                    {
                        aErrorList->append("Hint: �������� �������������� \""+aFormat->condition+"\" ������ ����� ���������� ��������");
                    }
                }
            }
        }
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
    fullDialog=new FullDialog(this);
    fullDialog->pageComponent=this;

    fullDialog->setWindowTitle(ui->nameEdit->text());

    fullDialog->addAction(mainWindow->ui->actionGlobalVars);
    fullDialog->addAction(mainWindow->ui->actionExport);
    fullDialog->addAction(mainWindow->ui->actionGenerateWord);
    fullDialog->addAction(mainWindow->ui->actionSectionControl);
    fullDialog->addAction(mainWindow->ui->actionSave);
    fullDialog->addAction(mainWindow->ui->actionSaveAs);
    fullDialog->addAction(mainWindow->ui->actionFind);
    fullDialog->addAction(mainWindow->ui->actionReplace);
    fullDialog->addAction(mainWindow->ui->actionFindNext);
    fullDialog->addAction(mainWindow->ui->actionFindPrev);

    //------------------------------------------

    ui->mainVerticalLayout->removeWidget(ui->expandedWidget);

    ui->expandButton->setVisible(false);
    ui->lockButton->setVisible(false);

    bool wasVisible=ui->useCheckBox->isChecked();
    ui->useCheckBox->setChecked(true);
    ui->useCheckBox->setVisible(false);
    ui->titleLabel->setVisible(false);

    ui->expandedWidget->setParent(fullDialog);
    ui->expandedWidget->show();
    fullDialog->ui->verticalLayout->addWidget(ui->expandedWidget);

    fullDialog->exec();

    //------------------------------------------

    ui->expandButton->setVisible(true);
    ui->lockButton->setVisible(true);

    ui->useCheckBox->setChecked(wasVisible);
    ui->useCheckBox->setVisible(mIsTable);
    ui->titleLabel->setVisible(!mIsTable);

    ui->expandedWidget->setParent(this);
    ui->expandedWidget->show();
    ui->mainVerticalLayout->addWidget(ui->expandedWidget);

    delete fullDialog;
    fullDialog=0;

    searchDialog=0;
}

void VariableExtendedListFrame::on_configButton_clicked()
{
    mainWindow->wantAutoSave=false;

    TableEditDialog dialog(this, this);
    dialog.exec();

    mainWindow->wantAutoSave=true;
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
                if (((IntegerColumn*)typeColumns.at(i).column)->mSplitRows)
                {
                    aItem->setText(QString::number(((IntegerColumn*)typeColumns.at(i).column)->mDefaultValue, 'f', ((IntegerColumn*)typeColumns.at(i).column)->mDecimals));
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

    dialog.setWindowTitle("�������");
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(1);
    dialog.setIntMaximum(10000);
    dialog.setIntValue(1);
    dialog.setLabelText("���������� �����:");

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

    dialog.setWindowTitle("�������");
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(1);
    dialog.setIntMaximum(10000);
    dialog.setIntValue(1);
    dialog.setLabelText("���������� �����:");

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

    dialog.setWindowTitle("�������");
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(1);
    dialog.setIntMaximum(10000);
    dialog.setIntValue(1);
    dialog.setLabelText("���������� �����:");

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

    dialog.setWindowTitle("�������");
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(1);
    dialog.setIntMaximum(10000);
    dialog.setIntValue(1);
    dialog.setLabelText("���������� �����:");

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
        QMessageBox::information(this, protocolCreatorVersion, "�������� ������");
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
        QMessageBox::information(this, protocolCreatorVersion, "�������� ������");
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

void VariableExtendedListFrame::on_clearButton_clicked()
{
    if (QMessageBox::question(this, protocolCreatorVersion, "�� �������, ��� ������ �������� �������?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::Yes)
    {
        ui->dataTableWidget->clearContents();
        ui->dataTableWidget->setRowCount(0);
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
            QMessageBox::information(this, "������� �� ������������ ������", "��� �������� � �������� ����������� ������");
            return;
        }

        if (aFrame->ui->dataTableWidget->rowCount()==0 || aFrame->ui->dataTableWidget->columnCount()==0)
        {
            QMessageBox::information(this, "������� �� ������������ ������", "��� ������ � ��������� �������");
            return;
        }

        InsertLinkTableDialog dialog(aFrame, this, this);
        dialog.exec();
    }
    else
    {
        if (mLinkForAnotherList=="")
        {
            QMessageBox::information(this, "������� �� ������������ ������", "���������� �������� � ������������ ������");
        }
        else
        {
            QMessageBox::information(this, "������� �� ������������ ������", "�� ������� �������� � ������������ ������ \""+mLinkForAnotherList+"\"");
        }
    }
}

void VariableExtendedListFrame::on_copyFromAnotherTableButton_clicked()
{
    VariableExtendedListFrame *aFrame=0;

    QString aLink=mLinkForCopyingAnotherList;

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
        DataTable *aSrcTable=aFrame->ui->dataTableWidget;

        if (ui->dataTableWidget->columnCount()==0)
        {
            QMessageBox::information(this, "����������� �� ������������ ������", "��� �������� � �������� ����������� ������");
            return;
        }

        if (aSrcTable->rowCount()==0 || aSrcTable->columnCount()==0)
        {
            QMessageBox::information(this, "����������� �� ������������ ������", "��� ������ � ��������� �������");
            return;
        }

        /////////////////////////////////
        //      START COPYING TABLE
        /////////////////////////////////

        if (mCopyColumnCount<1)
        {
            mCopyColumnCount=1;
        }
        else
        if (mCopyColumnCount>aSrcTable->columnCount())
        {
            mCopyColumnCount=aSrcTable->columnCount();
        }

        for (int i=0; i<aSrcTable->rowCount(); ++i)
        {
            if (aSrcTable->itemDelegateForRow(i))
            {
                if (mCopyMiddleRow)
                {
                    on_addMiddleRowButton_clicked();

                    ui->dataTableWidget->item(ui->dataTableWidget->rowCount()-1, 0)->setText(aSrcTable->item(i,0)->text());
                }
            }
            else
            {
                bool needAddRow=true;

                if (mCopyRules==crWithText)
                {
                    needAddRow=false;

                    for (int j=qMax(0, mRulesColumn-1); j<aSrcTable->columnCount(); ++j)
                    {
                        if (
                            mRulesColumn==0
                            ||
                            j==(mRulesColumn-1)
                           )
                        {
                            for (int k=0; k<mRulesText.length(); ++k)
                            {
                                if (aSrcTable->item(i, j)->text().contains(mRulesText.at(k), Qt::CaseInsensitive))
                                {
                                    needAddRow=true;
                                    break;
                                }
                            }

                            if (needAddRow)
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                if (mCopyRules==crWithoutText)
                {
                    for (int j=qMax(0, mRulesColumn-1); j<aSrcTable->columnCount(); ++j)
                    {
                        if (
                            mRulesColumn==0
                            ||
                            j==(mRulesColumn-1)
                           )
                        {
                            for (int k=0; k<mRulesText.length(); ++k)
                            {
                                if (aSrcTable->item(i, j)->text().contains(mRulesText.at(k), Qt::CaseInsensitive))
                                {
                                    needAddRow=false;
                                    break;
                                }
                            }

                            if (!needAddRow)
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                if (needAddRow)
                {
                    on_addRowButton_clicked();

                    for (int j=0; j<mCopyColumnCount; ++j)
                    {
                        QString aText=aSrcTable->item(i, j)->text();
                        QTableWidgetItem *aItem=ui->dataTableWidget->item(ui->dataTableWidget->rowCount()-1, j);

                        switch (typeColumns.at(j).column->type())
                        {
                            case ctInteger:
                            {
                                if (!((IntegerColumn*)(typeColumns.at(j).column))->mIsAutoInc)
                                {
                                    while (aText.length()>0 && !aText.at(aText.length()-1).isNumber())
                                    {
                                        aText.remove(aText.length()-1, 1);
                                    }

                                    while (aText.length()>0 && !aText.at(0).isNumber())
                                    {
                                        aText.remove(0, 1);
                                    }

                                    bool ok;
                                    double aValue=aText.toDouble(&ok);

                                    if (ok)
                                    {
                                        if (((IntegerColumn*)(typeColumns.at(j).column))->mSplitRows)
                                        {
                                            aItem->setText(QString::number(aValue, 'f', ((IntegerColumn*)(typeColumns.at(j).column))->mDecimals));
                                        }
                                        else
                                        {
                                            aItem->setText(
                                                           ((IntegerColumn*)(typeColumns.at(j).column))->mPrefix+
                                                           QString::number(aValue, 'f', ((IntegerColumn*)(typeColumns.at(j).column))->mDecimals)+
                                                           ((IntegerColumn*)(typeColumns.at(j).column))->mPostfix
                                                          );
                                        }
                                    }
                                }
                            }
                            break;
                            case ctString:
                            case ctList:
                            case ctExtendedList:
                            {
                                aItem->setText(aText);
                            }
                            break;
                            case ctBool:
                            {
                                if (aText=="1")
                                {
                                    aItem->setCheckState(Qt::Checked);
                                }
                                else
                                if (aText=="0")
                                {
                                    aItem->setCheckState(Qt::Unchecked);
                                }
                            }
                            break;
                            case ctDate:
                            {
                                QDate aDate=QDate::fromString(aText, "dd.MM.yyyy");

                                if (aDate.isValid())
                                {
                                    aItem->setText(aText);
                                }
                            }
                            break;
                            case ctTime:
                            {
                                QTime aTime=QTime::fromString(aText, "h:mm:ss");

                                if (aTime.isValid())
                                {
                                    aItem->setText(aTime.toString("hh:mm:ss"));
                                }
                            }
                            break;
                            case ctExpression:
                            {
                                if (((ExpressionColumn*)(typeColumns.at(j).column))->mAllowModify)
                                {
                                    aItem->setText(aText);
                                }
                            }
                            break;
                            default:
                            {
                                throw "Unknown column type";
                            }
                        }
                    }
                }
            }
        }

        /////////////////////////////////
        //       END COPYING TABLE
        /////////////////////////////////

        QMessageBox::information(this, "����������� �� ������������ ������", "������� ���������� ����������� �� \""+mLinkForCopyingAnotherList+"\"");
    }
    else
    {
        if (mLinkForCopyingAnotherList=="")
        {
            QMessageBox::information(this, "����������� �� ������������ ������", "���������� �������� � ������������ ������");
        }
        else
        {
            QMessageBox::information(this, "����������� �� ������������ ������", "�� ������� �������� � ������������ ������ \""+mLinkForCopyingAnotherList+"\"");
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
    setGeometryInDesktop(mCellAlignmentWidget,
                         cursor().pos().x()+5,
                         cursor().pos().y()+5,
                         mCellAlignmentWidget->width(),
                         mCellAlignmentWidget->height());

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
    if (!ui->controlWidget->isVisible())
    {
        return;
    }

    bool itemSelected=ui->dataTableWidget->selectedItems().length()>0;
    QString aClipboard=QApplication::clipboard()->text().replace("\r","");

    QMenu *contextMenu=new QMenu(this);

    contextMenu->addAction("�������� ������ ����� �������",               this, SLOT(dataTableInsertRowBefore()))->setEnabled(itemSelected);
    contextMenu->addAction("�������� ������ ����� �������",               this, SLOT(dataTableInsertRowAfter()))->setEnabled(itemSelected);
    contextMenu->addAction("�������� ������������� ������ ����� �������", this, SLOT(dataTableInsertMiddleRowBefore()))->setEnabled(itemSelected);
    contextMenu->addAction("�������� ������������� ������ ����� �������", this, SLOT(dataTableInsertMiddleRowAfter()))->setEnabled(itemSelected);
    contextMenu->addSeparator();

    QMenu *copyMenu=contextMenu->addMenu("�����������");
    copyMenu->addAction("���������� ������ � �����",                   this, SLOT(dataTableCopyRows()))->setEnabled(itemSelected);
    copyMenu->addAction("�������� ������ �� ������ ����� �������",     this, SLOT(dataTablePasteBefore()))->setEnabled(itemSelected && aClipboard!="");
    copyMenu->addAction("�������� ������ �� ������ ����� �������",     this, SLOT(dataTablePasteAfter()))->setEnabled(itemSelected && aClipboard!="");

    contextMenu->addSeparator();
    contextMenu->addAction("������� ������(�)",                           this, SLOT(on_deleteRowButton_clicked()))->setEnabled(itemSelected);
    contextMenu->addSeparator();

    QMenu *formatMenu=contextMenu->addMenu("������");
    formatMenu->addAction("�����",                                       this, SLOT(dataTableFont()))->setEnabled(itemSelected);
    formatMenu->addAction("���� ������",                                 this, SLOT(dataTableBackgroundColor()))->setEnabled(itemSelected);
    formatMenu->addAction("���� ������",                                 this, SLOT(dataTableTextColor()))->setEnabled(itemSelected);
    formatMenu->addSeparator();

    QMenu *cellAlignMenu=formatMenu->addMenu("��������� � ������");

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
