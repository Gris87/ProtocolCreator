#include "src/other/global.h"

ComponentTextFrame::ComponentTextFrame(QWidget *parent) :
    PageComponent(parent),
    ui(new Ui::ComponentTextFrame)
{
    ui->setupUi(this);

    ui->nameEdit->setText("Текст");
    ui->varNameEdit->setText("Text");
}

ComponentTextFrame::~ComponentTextFrame()
{
    delete ui;
}

void ComponentTextFrame::saveToStream(QDataStream &aStream)
{
    aStream << QString("ComponentText");

    aStream << QString("Name");
    aStream << ui->nameEdit->text();

    aStream << QString("VarName");
    aStream << ui->varNameEdit->text();

    bool aUsed=ui->useCheckBox->isChecked();

    aStream << QString("Used");
    aStream << aUsed;

    QString aValue=ui->valueEdit->toHtml();

    aStream << QString("Value");
    aStream << aValue;

    aStream << QString("VarEnd");
}

void ComponentTextFrame::loadFromStream(QDataStream &aStream)
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
        if (aMagicWord=="Value")
        {
            QString aValue;

            aStream >> aValue;
            ui->valueEdit->setHtml(aValue);
        }
        else
        if (aMagicWord=="VarEnd")
        {
            break;
        }
    }
}

void ComponentTextFrame::updateAdmin()
{
    ui->adminGroupBox->setVisible(isAdmin);
}

void ComponentTextFrame::on_nameEdit_textChanged(const QString &aName)
{
    ui->adminGroupBox->setTitle(aName);
    ui->useCheckBox->setText(aName);
}

void ComponentTextFrame::on_useCheckBox_toggled(bool checked)
{
    ui->userWidget->setVisible(checked);
}

void ComponentTextFrame::on_valueEdit_cursorPositionChanged()
{
    ui->leftButton->setFlat(ui->valueEdit->alignment()!=Qt::AlignLeft);
    ui->centerButton->setFlat(ui->valueEdit->alignment()!=Qt::AlignCenter);
    ui->rightButton->setFlat(ui->valueEdit->alignment()!=Qt::AlignRight);
    ui->wholeButton->setFlat(ui->valueEdit->alignment()!=Qt::AlignJustify);
}

void ComponentTextFrame::on_cutButton_clicked()
{
    ui->valueEdit->cut();

    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_copyButton_clicked()
{
    ui->valueEdit->copy();

    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_pasteButton_clicked()
{
    ui->valueEdit->paste();

    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_undoButton_clicked()
{
    ui->valueEdit->undo();

    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_redoButton_clicked()
{
    ui->valueEdit->redo();

    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_fontButton_clicked()
{
    QFont aFont=ui->valueEdit->currentFont();

    QFontDialog dialog(aFont, this);

    if (dialog.exec())
    {
        ui->valueEdit->setCurrentFont(dialog.selectedFont());
    }

    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_boldButton_clicked()
{
    ui->valueEdit->setFontWeight(125-ui->valueEdit->fontWeight());

    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_italicButton_clicked()
{
    ui->valueEdit->setFontItalic(!ui->valueEdit->fontItalic());

    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_underlineButton_clicked()
{
    ui->valueEdit->setFontUnderline(!ui->valueEdit->fontUnderline());

    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_leftButton_clicked()
{
    ui->valueEdit->setAlignment(Qt::AlignLeft);

    on_valueEdit_cursorPositionChanged();
    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_centerButton_clicked()
{
    ui->valueEdit->setAlignment(Qt::AlignCenter);

    on_valueEdit_cursorPositionChanged();
    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_rightButton_clicked()
{
    ui->valueEdit->setAlignment(Qt::AlignRight);

    on_valueEdit_cursorPositionChanged();
    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_wholeButton_clicked()
{
    ui->valueEdit->setAlignment(Qt::AlignJustify);

    on_valueEdit_cursorPositionChanged();
    ui->valueEdit->setFocus();
}

void ComponentTextFrame::on_valueEdit_copyAvailable(bool available)
{
    ui->cutButton->setEnabled(available);
    ui->copyButton->setEnabled(available);
}

void ComponentTextFrame::on_valueEdit_undoAvailable(bool available)
{
    ui->undoButton->setEnabled(available);
}

void ComponentTextFrame::on_valueEdit_redoAvailable(bool available)
{
    ui->redoButton->setEnabled(available);
}

void ComponentTextFrame::on_valueEdit_currentCharFormatChanged(const QTextCharFormat &format)
{
    ui->boldButton->setFlat(ui->valueEdit->fontWeight()==50);
    ui->italicButton->setFlat(!ui->valueEdit->fontItalic());
    ui->underlineButton->setFlat(!ui->valueEdit->fontUnderline());
}

void ComponentTextFrame::on_valueEdit_currentColorChanged(const QColor &aColor)
{
    QPalette aPalette=ui->colorArea->palette();

    aPalette.setColor(QPalette::Window, aColor);

    ui->colorArea->setPalette(aPalette);
}

void ComponentTextFrame::on_colorArea_colorChanged(const QColor &aColor)
{
    ui->valueEdit->setTextColor(aColor);
}
