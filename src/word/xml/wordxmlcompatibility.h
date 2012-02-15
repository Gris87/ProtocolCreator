#ifndef WORDXMLCOMPATIBILITY_H
#define WORDXMLCOMPATIBILITY_H

#include <QTextStream>

class WordXMLCompatibility
{
public:
    bool isBreakWrappedTables;
    bool isSnapToGridInCell;
    bool isWrapTextWithPunct;
    bool isUseAsianBreakRules;
    bool isDontGrowAutofit;
    bool isUseFELayout;

    explicit WordXMLCompatibility();
    ~WordXMLCompatibility();

    void writeToStream(QTextStream &aStream);

    void reset();
};

#endif // WORDXMLCOMPATIBILITY_H
