#ifndef COLORPANEL_H
#define COLORPANEL_H

#include <QScrollArea>

#include <QMouseEvent>
#include <QColorDialog>

class ColorPanel : public QScrollArea
{
    Q_OBJECT
public:
    explicit ColorPanel(QWidget *parent = 0);
    ~ColorPanel();

    void mousePressEvent(QMouseEvent *event);

signals:
    void colorChanged(const QColor &aColor);
};

#endif // COLORPANEL_H
