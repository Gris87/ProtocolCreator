#ifndef WORDEDITFRAME_H
#define WORDEDITFRAME_H

#include <QWidget>

#include "ui_wordeditframe.h"

#include <QFontDialog>
#include <QMenu>

namespace Ui {
    class WordEditFrame;
}

class WordEditFrame : public QWidget
{
    Q_OBJECT

public:
    Ui::WordEditFrame *ui;

    explicit WordEditFrame(QWidget *parent = 0);
    ~WordEditFrame();

private slots:
    void on_valueEdit_cursorPositionChanged();
    void on_cutButton_clicked();
    void on_copyButton_clicked();
    void on_pasteButton_clicked();
    void on_undoButton_clicked();
    void on_redoButton_clicked();
    void on_fontButton_clicked();
    void on_fontComboBox_currentIndexChanged(const QString &aFont);
    void on_fontSizeSpinBox_valueChanged(int aSize);
    void on_boldButton_clicked();
    void on_italicButton_clicked();
    void on_underlineButton_clicked();
    void on_strikeOutButton_clicked();
    void on_leftButton_clicked();
    void on_centerButton_clicked();
    void on_rightButton_clicked();
    void on_wholeButton_clicked();
    void on_valueEdit_copyAvailable(bool available);
    void on_valueEdit_undoAvailable(bool available);
    void on_valueEdit_redoAvailable(bool available);
    void on_valueEdit_currentCharFormatChanged(const QTextCharFormat &format);
    void on_textColorArea_colorChanged(const QColor &aColor);
    void on_backgroundColorArea_colorChanged(const QColor &aColor);
    void insertPage();
    void insertPageCount();
    void on_contextButton_clicked();
};

#endif // WORDEDITFRAME_H
