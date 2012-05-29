#include "src/other/global.h"

WordEditFrame::WordEditFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordEditFrame)
{
    ui->setupUi(this);

    ui->textColorArea->setColor(QColor(0, 0, 0));
    ui->backgroundColorArea->setColor(QColor(255, 255, 255));
    ui->frameColorArea->setColor(QColor(255, 255, 255));

    ui->valueEdit->setTabStopWidth(50);

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



    QPalette aPalette=ui->textColorArea->palette();

    if (ui->valueEdit->textCursor().blockFormat().background().style()==Qt::NoBrush)
    {
        aPalette.setColor(QPalette::Window, QColor(255, 255, 255));
    }
    else
    {
        aPalette.setColor(QPalette::Window, ui->valueEdit->textCursor().blockFormat().background().color());
    }

    ui->frameColorArea->setPalette(aPalette);
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

void WordEditFrame::on_fontComboBox_currentIndexChanged(const QString &aFont)
{
    QTextCharFormat modifier;
    modifier.setFontFamily(aFont);

    ui->valueEdit->mergeCurrentCharFormat(modifier);
}

void WordEditFrame::on_fontSizeSpinBox_valueChanged(int aSize)
{
    QTextCharFormat modifier;
    modifier.setFontPointSize(aSize);

    ui->valueEdit->mergeCurrentCharFormat(modifier);
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

void WordEditFrame::on_strikeOutButton_clicked()
{
    QTextCharFormat modifier;
    modifier.setFontStrikeOut(!ui->valueEdit->currentCharFormat().fontStrikeOut());

    ui->valueEdit->mergeCurrentCharFormat(modifier);

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
    ui->strikeOutButton->setFlat(!format.fontStrikeOut());

    disconnect(ui->fontComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_fontComboBox_currentIndexChanged(QString)));
    disconnect(ui->fontSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_fontSizeSpinBox_valueChanged(int)));

    ui->fontComboBox->setCurrentFont(format.font());
    ui->fontSizeSpinBox->setValue((int)format.fontPointSize());

    connect(ui->fontComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_fontComboBox_currentIndexChanged(QString)));
    connect(ui->fontSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_fontSizeSpinBox_valueChanged(int)));

    QPalette aPalette=ui->textColorArea->palette();
    aPalette.setColor(QPalette::Window, format.foreground().color());
    ui->textColorArea->setPalette(aPalette);

    aPalette=ui->backgroundColorArea->palette();

    if (format.background().style()==Qt::NoBrush)
    {
        aPalette.setColor(QPalette::Window, QColor(255, 255, 255));
    }
    else
    {
        aPalette.setColor(QPalette::Window, format.background().color());
    }

    ui->backgroundColorArea->setPalette(aPalette);
}

void WordEditFrame::on_textColorArea_colorChanged(const QColor &aColor)
{
    ui->valueEdit->setTextColor(aColor);
}

void WordEditFrame::on_backgroundColorArea_colorChanged(const QColor &aColor)
{
    ui->valueEdit->setTextBackgroundColor(aColor);
}

void WordEditFrame::on_frameColorArea_colorChanged(const QColor &aColor)
{
    QTextCursor aCursor=ui->valueEdit->textCursor();

    int start=aCursor.selectionStart();
    int end=aCursor.selectionEnd();

    for (int i=start; i<=end; i++)
    {
        aCursor.setPosition(i);

        QTextBlockFormat aFormat=aCursor.blockFormat();
        aFormat.setBackground(QBrush(aColor));
        aCursor.setBlockFormat(aFormat);
    }
}

void WordEditFrame::insertPage()
{
    ui->valueEdit->insertPlainText("#PAGE#");
}

void WordEditFrame::insertPageCount()
{
    ui->valueEdit->insertPlainText("#NUMPAGES#");
}

void WordEditFrame::insertLink()
{
    ComponentSelectionDialog dialog(this);

    if (dialog.exec())
    {
        QString aSection=dialog.mResult;

        aSection=aSection.left(aSection.indexOf("."));

        if (
            aSection=="Global"
            ||
            (
             !globalDialog->isVisible()
             &&
             aSection==((PageFrame*)mainWindow->ui->pagesTabWidget->currentWidget())->ui->varNameEdit->text()
            )
           )
        {
            dialog.mResult=dialog.mResult.mid(dialog.mResult.indexOf(".")+1);
        }

        ui->valueEdit->insertPlainText("["+dialog.mResult+"]");
    }
}

void WordEditFrame::on_contextButton_clicked()
{
    QMenu *contextMenu=new QMenu;

    QMenu *autoTextMenu=contextMenu->addMenu("Вставить авто-текст");

    autoTextMenu->addAction("Страница", this, SLOT(insertPage()));
    autoTextMenu->addAction("Количество страниц", this, SLOT(insertPageCount()));

    contextMenu->addAction("Вставить переменную", this, SLOT(insertLink()));

    setGeometryInDesktop(contextMenu,
                         cursor().pos().x(),
                         cursor().pos().y(),
                         contextMenu->sizeHint().width(),
                         contextMenu->sizeHint().height());

    contextMenu->show();
}
