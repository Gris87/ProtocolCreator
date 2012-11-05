#include "colorarea.h"

ColorArea::ColorArea(QWidget *parent) :
    QFrame(parent)
{
    QSizePolicy aPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(aPolicy);

    setFrameShadow(QFrame::Sunken);
    setFrameShape(QFrame::StyledPanel);

    needDrawFrame=true;
    popupAllowed=true;
    selectAllowed=true;
    popupCount=9;
    popupCellSize=9;
}

void ColorArea::setColor(QColor aColor)
{
    QPalette aPalette;

    aPalette.setColor(QPalette::Window, aColor);
    setPalette(aPalette);
}

QColor ColorArea::color()
{
    return palette().window().color();
}

void ColorArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
        if (selectAllowed)
        {
            QColorDialog dialog(color(), this);

            if (dialog.exec())
            {
                setColor(dialog.selectedColor());
                emit colorChanged(dialog.selectedColor());
            }
        }
        else
        {
            emit clicked(this);
        }
    }
    else
    if (event->button()==Qt::RightButton)
    {
        if (popupAllowed)
        {
            aPopupWidget=new QWidget(this, Qt::Popup);

            int aWidthSize=popupCellSize*popupCount*popupCount+4;
            int aHeightSize=popupCellSize*popupCount+4;

            int aX=cursor().pos().x();
            int aY=cursor().pos().y();

            QDesktopWidget *desktop = QApplication::desktop();
            QRect aScreenRect=desktop->screenGeometry();
            int aWidth = aScreenRect.right();
            int aHeight = aScreenRect.bottom();

            if (aX+aWidthSize>aWidth)
            {
                aX=aWidth-aWidthSize;
            }

            if (aY+aHeightSize>aHeight)
            {
                aY=aHeight-aHeightSize;
            }

            aPopupWidget->setGeometry(aX, aY, aWidthSize, aHeightSize);

            QGridLayout *aLayout=new QGridLayout(aPopupWidget);
            aLayout->setHorizontalSpacing(0);
            aLayout->setVerticalSpacing(0);
            aLayout->setContentsMargins(2, 2, 2, 2);

            quint16 aDiffY=255/(popupCount-1);
            quint32 aDiffX=65535/(popupCount*popupCount-1);

            for (int i=0; i<popupCount; i++)
            {
                for (int j=0; j<popupCount*popupCount; j++)
                {
                    ColorArea *aArea=new ColorArea(this);

                    aArea->popupAllowed=false;
                    aArea->selectAllowed=false;
                    aArea->needDrawFrame=false;

                    aArea->setColor(QColor(aDiffY*i, aDiffX*j>>8, aDiffX*j & 0xFF));

                    connect(aArea, SIGNAL(clicked(ColorArea*)), this, SLOT(cellClicked(ColorArea*)));

                    aLayout->addWidget(aArea, i, j);
                }
            }

            aPopupWidget->show();
        }
        else
        {
            emit rightClicked(this);
        }
    }
}

void ColorArea::paintEvent(QPaintEvent * /*event*/)
{
    QPainter paint(this);
    paint.setBrush(QBrush(color()));
    paint.drawRect(0,0,width(), height());

    if (needDrawFrame)
    {
        drawFrame(&paint);
    }
}

void ColorArea::cellClicked(ColorArea *aArea)
{
    setColor(aArea->color());
    emit colorChanged(aArea->color());

    delete aPopupWidget;
}
