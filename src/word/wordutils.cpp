#include "wordutils.h"

QString colorToString(QColor aColor)
{
    QString res="";

    res=QString::number(aColor.red(), 16).toUpper();

    if (res.length()<2)
    {
        res.insert(0, "0");
    }

    res.insert(0, QString::number(aColor.green(), 16).toUpper());

    if (res.length()<4)
    {
        res.insert(0, "0");
    }

    res.insert(0, QString::number(aColor.blue(), 16).toUpper());

    if (res.length()<6)
    {
        res.insert(0, "0");
    }

    return res;
}
