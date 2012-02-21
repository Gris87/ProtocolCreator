#include "wordutils.h"

QString colorToString(QColor aColor)
{
    QString res="";

    res=QString::number(aColor.blue(), 16).toUpper();

    if (res.length()<2)
    {
        res.insert(0, "0");
    }

    res.insert(0, QString::number(aColor.green(), 16).toUpper());

    if (res.length()<4)
    {
        res.insert(0, "0");
    }

    res.insert(0, QString::number(aColor.red(), 16).toUpper());

    if (res.length()<6)
    {
        res.insert(0, "0");
    }

    return res;
}

QString colorToStringWithRound(QColor aColor)
{
    QString res="";
    int minDistance=INT_MAX;

    int curDistance;

    curDistance=distanceBetweenColors(aColor, QColor(0, 0, 0));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="black";
    }

    curDistance=distanceBetweenColors(aColor, QColor(255, 255, 255));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="white";
    }

    curDistance=distanceBetweenColors(aColor, QColor(255, 0, 0));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="red";
    }

    curDistance=distanceBetweenColors(aColor, QColor(0, 255, 0));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="green";
    }

    curDistance=distanceBetweenColors(aColor, QColor(0, 0, 255));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="blue";
    }

    curDistance=distanceBetweenColors(aColor, QColor(255, 255, 0));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="yellow";
    }

    curDistance=distanceBetweenColors(aColor, QColor(255, 0, 255));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="magenta";
    }

    curDistance=distanceBetweenColors(aColor, QColor(0, 255, 255));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="cyan";
    }

    curDistance=distanceBetweenColors(aColor, QColor(127, 0, 0));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="dark-red";
    }

    curDistance=distanceBetweenColors(aColor, QColor(0, 127, 0));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="dark-green";
    }

    curDistance=distanceBetweenColors(aColor, QColor(0, 0, 127));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="dark-blue";
    }

    curDistance=distanceBetweenColors(aColor, QColor(127, 127, 0));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="dark-yellow";
    }

    curDistance=distanceBetweenColors(aColor, QColor(127, 0, 127));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="dark-magenta";
    }

    curDistance=distanceBetweenColors(aColor, QColor(0, 127, 127));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="dark-cyan";
    }

    curDistance=distanceBetweenColors(aColor, QColor(191, 191, 191));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="light-gray";
    }

    curDistance=distanceBetweenColors(aColor, QColor(127, 127, 127));

    if (curDistance<minDistance)
    {
        minDistance=curDistance;
        res="dark-gray";
    }

    return res;
}

int distanceBetweenColors(QColor aColor1, QColor aColor2)
{
    int rDelta=aColor1.red()  -aColor2.red();
    int gDelta=aColor1.green()-aColor2.green();
    int bDelta=aColor1.blue() -aColor2.blue();

    return rDelta*rDelta+gDelta*gDelta+bDelta*bDelta;
}
