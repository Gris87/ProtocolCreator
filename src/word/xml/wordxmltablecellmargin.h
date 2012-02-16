#ifndef WORDXMLTABLECELLMARGIN_H
#define WORDXMLTABLECELLMARGIN_H

#include "wordxmlbase.h"

class WordXMLTableCellMargin : public WordXMLBase
{
public:
    double top;
    double left;
    double bottom;
    double right;

    explicit WordXMLTableCellMargin(WordXMLBase* aParent=0);
    ~WordXMLTableCellMargin();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    void setMargin(double aTop, double aLeft, double aBottom, double aRight);
};

#endif // WORDXMLTABLECELLMARGIN_H
