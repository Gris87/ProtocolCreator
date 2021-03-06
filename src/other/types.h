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

struct SConditionFormat
{
    QString fontString;
    quint8 alignment;
    quint8 backgroundColorR;
    quint8 backgroundColorG;
    quint8 backgroundColorB;
    quint8 textColorR;
    quint8 textColorG;
    quint8 textColorB;
    QString condition;
    bool needWarning;
};

struct STableColumn
{
    QString name;
    bool visible;
    ColumnType *column;
    double leftMargin;
    double rightMargin;
    double topMargin;
    double bottomMargin;
    QString fontString;
    quint8 alignment;
    quint8 backgroundColorR;
    quint8 backgroundColorG;
    quint8 backgroundColorB;
    quint8 textColorR;
    quint8 textColorG;
    quint8 textColorB;
    QList<SConditionFormat> conditions;
};

#endif // TYPES_H
