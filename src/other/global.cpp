#include "src/other/global.h"

// FORMS
MainWindow                *mainWindow=0;
SearchDialog              *searchDialog=0;
GlobalDialog              *globalDialog=0;
FullDialog                *fullDialog=0;

// GENERAL
bool                       isAdmin=true;
QString                    currentName="";
QString                    docPass="";
QString                    adminPass="";

// OTHERS
QString                    dir="";
QStringList                functionsList;

QString                    protocolCreatorVersion="Protocol Creator v 1.01";

QString                    lastSearch="";
QString                    lastReplace="";
bool                       isReplace=false;
bool                       isFindAll=false;
