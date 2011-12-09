#include "word.h"

WordApp::WordApp(QObject *parent) :
    WordBase(parent)
{
    base_ax_object=new QAxObject("Word.Application", this);
}

WordApp::~WordApp()
{
}

int WordApp::defaultFileFormat()
{
    try
    {
        return base_ax_object->dynamicCall("DefaultSaveFormat()").toInt();
    }
    catch (...)
    {
        qWarning()<<"WordApp::defaultFileFormat(): Couldn't receive defaultFileFormat";
    }

    return 0;
}

WordOptions* WordApp::options()
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Options()");

        if (aObject==0)
        {
            qWarning()<<"WordApp::options(): Retry #"+QString::number(retry+1)+" Couldn't receive options";
            continue;
        }

        return new WordOptions(aObject, this);
    }

    throw "WordApp::options(): Couldn't receive options";
}

WordDocuments* WordApp::documents()
{
    for (int retry=0; retry<WORD_MAX_RETRIES; retry++)
    {
        QAxObject *aObject=base_ax_object->querySubObject("Documents()");

        if (aObject==0)
        {
            qWarning()<<"WordApp::documents(): Retry #"+QString::number(retry+1)+" Couldn't receive documents";
            continue;
        }

        return new WordDocuments(aObject, this);
    }

    throw "WordApp::documents(): Couldn't receive documents";
}

void WordApp::setVisible(bool aVisible)
{
    try
    {
        base_ax_object->dynamicCall("SetVisible(bool)", aVisible);
    }
    catch (...)
    {
        qWarning()<<"WordApp::setVisible(): Couldn't change word application visibility";
    }
}

void WordApp::quit()
{
    try
    {
        base_ax_object->dynamicCall("Quit()");
    }
    catch (...)
    {
        qWarning()<<"WordApp::quit(): Couldn't close word application";
    }
}

QString WordApp::version()
{
    try
    {
        return base_ax_object->dynamicCall("Version()").toString();
    }
    catch (...)
    {
        qWarning()<<"WordApp::version(): Couldn't receive version";
    }

    return "";
}
