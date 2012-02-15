#ifndef     WORDXMLBASE_H
#define     WORDXMLBASE_H

#include <QTextStream>

class WordXMLBase
{
public:
    explicit WordXMLBase();
    ~WordXMLBase();

    virtual void writeToStream(QTextStream &aStream);

    virtual void reset();

    virtual bool isModified();
};

#endif //     WORDXMLBASE_H
