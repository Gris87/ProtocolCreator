#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QColor>

struct STableCell
{
    int spanX;
    int spanY;
    QString fontString;
    QString text;
    quint8 alignment;
    quint8 backgroundColorR;
    quint8 backgroundColorG;
    quint8 backgroundColorB;
    quint8 textColorR;
    quint8 textColorG;
    quint8 textColorB;
};

#endif // TYPES_H
