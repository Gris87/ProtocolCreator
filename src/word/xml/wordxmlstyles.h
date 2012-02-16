#ifndef WORDXMLSTYLES_H
#define WORDXMLSTYLES_H

#include "wordxmlbase.h"

#include "wordxmlstyle.h"

class WordXMLStyles : public WordXMLBase
{
public:
    explicit WordXMLStyles(WordXMLBase* aParent=0);
    ~WordXMLStyles();

    void writeToStream(QTextStream &aStream);

    void reset();

    bool isModified();

    void setDefaultStyles2003();

    WordXMLStyle* add(QString aStyleID, EStyleType aType, bool isDefault);

    WordXMLStyle* copy(QString aStyleID, QString aSecondStyleID);
    WordXMLStyle* copy(int index, QString aSecondStyleID);
    WordXMLStyle* copy(WordXMLStyle* aStyle, QString aSecondStyleID);

    WordXMLStyle* get(QString aStyleID);
    WordXMLStyle* get(int index);

    void remove(QString aStyleID);
    void remove(int index);
    void remove(WordXMLStyle* aStyle);

    int count();

private:
    QList<WordXMLStyle*> mList;

    void clear();
};

#endif // WORDXMLSTYLES_H
