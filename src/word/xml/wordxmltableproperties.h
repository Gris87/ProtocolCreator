#ifndef WORDXMLTABLEPROPERTIES_H
#define WORDXMLTABLEPROPERTIES_H

#include <QTextStream>

#include "wordxmltablecellmargin.h"

class WordXMLTableProperties
{
public:
    double indentation;
    WordXMLTableCellMargin cellMargin;

    explicit WordXMLTableProperties();
    ~WordXMLTableProperties();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLTABLEPROPERTIES_H
