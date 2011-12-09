#ifndef WORDDOCUMENTS_H
#define WORDDOCUMENTS_H

#include "wordworkbase.h"
#include "worddocument.h"

#include <QDir>

class WordDocuments : public WordWorkBase
{
    Q_OBJECT
public:
    explicit WordDocuments(QAxObject *aBaseObject, QObject *parent = 0);

    WordDocument* add();
    WordDocument* open(QString aPath);
    int count();
};

#endif // WORDDOCUMENTS_H
