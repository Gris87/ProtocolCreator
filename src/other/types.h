#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QColor>

#include "src/column_types/columntype.h"

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

struct STableColumn
{
    QString name;
    ColumnType *column;
    double leftOffset;
    double rightOffset;
    double totalOffset;
    QString fontString;
    quint8 alignment;
    quint8 backgroundColorR;
    quint8 backgroundColorG;
    quint8 backgroundColorB;
    quint8 textColorR;
    quint8 textColorG;
    quint8 textColorB;
};

#endif // TYPES_H
