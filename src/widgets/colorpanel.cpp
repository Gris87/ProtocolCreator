#include "colorpanel.h"

ColorPanel::ColorPanel(QWidget *parent) :
    QScrollArea(parent)
{
}

ColorPanel::~ColorPanel()
{

}

void ColorPanel::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
        QColorDialog dialog(palette().color(QPalette::Window), this);

        if (dialog.exec())
        {
            emit colorChanged(dialog.selectedColor());
        }
    }
}
