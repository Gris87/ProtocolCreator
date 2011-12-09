#include "movabletabwidget.h"

MovableTabWidget::MovableTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
}

QTabBar* MovableTabWidget::tabBar()
{
    return QTabWidget::tabBar();
}
