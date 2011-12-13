#include "src/other/global.h"

WordEditFrame::WordEditFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordEditFrame)
{
    ui->setupUi(this);

    ui->colorArea->setColor(QColor(0, 0, 0));

    connect(ui->valueEdit, SIGNAL(boldCombination()), this, SLOT(on_boldButton_clicked()));
    connect(ui->valueEdit, SIGNAL(italicCombination()), this, SLOT(on_italicButton_clicked()));
    connect(ui->valueEdit, SIGNAL(underlineCombination()), this, SLOT(on_underlineButton_clicked()));
}

WordEditFrame::~WordEditFrame()
{
    delete ui;
}

void WordEditFrame::on_valueEdit_cursorPositionChanged()
{
    ui->leftButton->setFlat(ui->valueEdit->alignment()!=Qt::AlignLeft);
    ui->centerButton->setFlat(ui->valueEdit->alignment()!=Qt::AlignCenter);
    ui->rightButton->setFlat(ui->valueEdit->alignment()!=Qt::AlignRight);
    ui->wholeButton->setFlat(ui->valueEdit->alignment()!=Qt::AlignJustify);
}

void WordEditFrame::on_cutButton_clicked()
{
    ui->valueEdit->cut();

    ui->valueEdit->setFocus();
}

void WordEditFrame::on_copyButton_clicked()
{
    ui->valueEdit->copy();

    ui->valueEdit->setFocus();
}

void WordEditFrame::on_pasteButton_clicked()
{
    ui->valueEdit->paste();

    ui->valueEdit->setFocus();
}

void WordEditFrame::on_undoButton_clicked()
{
    ui->valueEdit->undo();

    ui->valueEdit->setFocus();
}

void WordEditFrame::on_redoButton_clicked()
{
    ui->valueEdit->redo();

    ui->valueEdit->setFocus();
}

void WordEditFrame::on_fontButton_clicked()
{
    QFont aFont=ui->valueEdit->currentFont();

    QFontDialog dialog(aFont, this);

    if (dialog.exec())
    {
        ui->valueEdit->setCurrentFont(dialog.selectedFont());
    }

    ui->valueEdit->setFocus();
}

void WordEditFrame::on_boldButton_clicked()
{
    ui->valueEdit->setFontWeight(125-ui->valueEdit->fontWeight());

    ui->valueEdit->setFocus();
}

void WordEditFrame::on_italicButton_clicked()
{
    ui->valueEdit->setFontItalic(!ui->valueEdit->fontItalic());

    ui->valueEdit->setFocus();
}

void WordEditFrame::on_underlineButton_clicked()
{
    ui->valueEdit->setFontUnderline(!ui->valueEdit->fontUnderline());

    ui->valueEdit->setFocus();
}

void WordEditFrame::on_leftButton_clicked()
{
    ui->valueEdit->setAlignment(Qt::AlignLeft);

    on_valueEdit_cursorPositionChanged();
    ui->valueEdit->setFocus();
}

void WordEditFrame::on_centerButton_clicked()
{
    ui->valueEdit->setAlignment(Qt::AlignCenter);

    on_valueEdit_cursorPositionChanged();
    ui->valueEdit->setFocus();
}

void WordEditFrame::on_rightButton_clicked()
{
    ui->valueEdit->setAlignment(Qt::AlignRight);

    on_valueEdit_cursorPositionChanged();
    ui->valueEdit->setFocus();
}

void WordEditFrame::on_wholeButton_clicked()
{
    ui->valueEdit->setAlignment(Qt::AlignJustify);

    on_valueEdit_cursorPositionChanged();
    ui->valueEdit->setFocus();
}

void WordEditFrame::on_valueEdit_copyAvailable(bool available)
{
    ui->cutButton->setEnabled(available);
    ui->copyButton->setEnabled(available);
}

void WordEditFrame::on_valueEdit_undoAvailable(bool available)
{
    ui->undoButton->setEnabled(available);
}

void WordEditFrame::on_valueEdit_redoAvailable(bool available)
{
    ui->redoButton->setEnabled(available);
}

void WordEditFrame::on_valueEdit_currentCharFormatChanged(const QTextCharFormat &format)
{
    ui->boldButton->setFlat(ui->valueEdit->fontWeight()==50);
    ui->italicButton->setFlat(!ui->valueEdit->fontItalic());
    ui->underlineButton->setFlat(!ui->valueEdit->fontUnderline());
}

void WordEditFrame::on_valueEdit_currentColorChanged(const QColor &aColor)
{
    QPalette aPalette=ui->colorArea->palette();

    aPalette.setColor(QPalette::Window, aColor);

    ui->colorArea->setPalette(aPalette);
}

void WordEditFrame::on_colorArea_colorChanged(const QColor &aColor)
{
    ui->valueEdit->setTextColor(aColor);
}
