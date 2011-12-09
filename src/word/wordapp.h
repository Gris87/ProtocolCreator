#ifndef WORDAPP_H
#define WORDAPP_H

#include "wordbase.h"
#include "wordoptions.h"
#include "worddocuments.h"

class WordApp : public WordBase
{
    Q_OBJECT
public:
    explicit WordApp(QObject *parent = 0);
    ~WordApp();

    int defaultFileFormat();

    WordOptions* options();

    WordDocuments* documents();

    void setVisible(bool aVisible);
    void quit();

    QString version();
};

#endif // WORDAPP_H
