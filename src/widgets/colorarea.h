#ifndef COLORAREA_H
#define COLORAREA_H

#include <QFrame>
#include <QPalette>
#include <QPainter>
#include <QScreen>
#include <QColorDialog>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QApplication>

class ColorArea : public QFrame
{
    Q_OBJECT
public:
    explicit ColorArea(QWidget *parent = 0);

    void setColor(QColor aColor);
    QColor color();

    bool needDrawFrame;
    bool popupAllowed;
    bool selectAllowed;
    quint8 popupCount;
    quint8 popupCellSize;

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

    QWidget *aPopupWidget;

private slots:
    void cellClicked(ColorArea *aArea);

signals:
    void clicked(ColorArea *aArea);
    void rightClicked(ColorArea *aArea);
    void colorChanged(const QColor &aColor);
};

#endif // COLORAREA_H
