#ifndef     WORDXMLBASE_H
#define     WORDXMLBASE_H

#include <QTextStream>

class WordXMLBase
{
public:
    QString space;
    WordXMLBase *parent;

    explicit WordXMLBase(WordXMLBase* aParent=0);
    ~WordXMLBase();

    virtual void writeToStream(QTextStream &aStream);

    virtual void reset();

    virtual bool isModified();
};

#endif //     WORDXMLBASE_H
