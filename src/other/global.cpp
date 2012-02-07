#include "src/other/global.h"

// FORMS
MainWindow                *mainWindow=0;
SearchDialog              *searchDialog=0;
GlobalDialog              *globalDialog=0;

// GENERAL
bool                       isAdmin=true;
QString                    currentName="";
QString                    docPass="";
QString                    adminPass="";

// OTHERS
QString                    dir="";
QStringList                functionsList;
double                     pixelToSantimeter=0.0263921875;

QString                    protocolCreatorVersion="Protocol Creator v 1.0";

QString                    lastSearch="";
QString                    lastReplace="";
bool                       isReplace=false;
