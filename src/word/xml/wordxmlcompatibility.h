#ifndef WORDXMLCOMPATIBILITY_H
#define WORDXMLCOMPATIBILITY_H

#include "wordxmlbase.h"

class WordXMLCompatibility : public WordXMLBase
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

    bool isModified();
};

#endif // WORDXMLCOMPATIBILITY_H
