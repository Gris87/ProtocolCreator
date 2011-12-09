#ifndef MOVABLETABWIDGET_H
#define MOVABLETABWIDGET_H

#include <QTabWidget>

class MovableTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit MovableTabWidget(QWidget *parent = 0);

    QTabBar* tabBar();
};

#endif // MOVABLETABWIDGET_H
