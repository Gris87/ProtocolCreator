#ifndef COMPONENTTEXTFRAME_H
#define COMPONENTTEXTFRAME_H

#include "src/components/pagecomponent.h"

#include "ui_componenttextframe.h"

#include <QFontDialog>

namespace Ui {
    class ComponentTextFrame;
}

class ComponentTextFrame : public PageComponent
{
    Q_OBJECT

public:
    Ui::ComponentTextFrame *ui;

    explicit ComponentTextFrame(QWidget *parent = 0);
    ~ComponentTextFrame();

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();

private slots:
    void on_nameEdit_textChanged(const QString &aName);
    void on_useCheckBox_toggled(bool checked);
    void on_valueEdit_cursorPositionChanged();
    void on_cutButton_clicked();
    void on_copyButton_clicked();
    void on_pasteButton_clicked();
    void on_undoButton_clicked();
    void on_redoButton_clicked();
    void on_fontButton_clicked();
    void on_boldButton_clicked();
    void on_italicButton_clicked();
    void on_underlineButton_clicked();
    void on_leftButton_clicked();
    void on_centerButton_clicked();
    void on_rightButton_clicked();
    void on_wholeButton_clicked();
    void on_valueEdit_copyAvailable(bool available);
    void on_valueEdit_undoAvailable(bool available);
    void on_valueEdit_redoAvailable(bool available);
    void on_valueEdit_currentCharFormatChanged(const QTextCharFormat &format);
    void on_valueEdit_currentColorChanged(const QColor &aColor);
    void on_colorArea_colorChanged(const QColor &aColor);
};

#endif // COMPONENTTEXTFRAME_H
