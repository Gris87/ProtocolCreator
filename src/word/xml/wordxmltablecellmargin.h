#ifndef WORDXMLTABLECELLMARGIN_H
#define WORDXMLTABLECELLMARGIN_H

#include <QTextStream>

class WordXMLTableCellMargin
{
public:
    double top;
    double left;
    double bottom;
    double right;

    explicit WordXMLTableCellMargin();
    ~WordXMLTableCellMargin();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    void setMargin(double aTop, double aLeft, double aBottom, double aRight);
};

#endif // WORDXMLTABLECELLMARGIN_H
