#ifndef GLOBAL_H
#define GLOBAL_H

#include "qdebug.h"

#include "src/components/pagecomponent.h"
#include "src/components/variableintegerframe.h"
#include "src/components/variablestringframe.h"
#include "src/components/variableboolframe.h"
#include "src/components/variabledateframe.h"
#include "src/components/variabletimeframe.h"
#include "src/components/variablelistframe.h"

#include "src/components/componenttextframe.h"

#include "src/main/headerfooterdialog.h"
#include "src/main/mainwindow.h"
#include "src/main/passworddialog.h"

#include "src/other/encoder.h"
#include "src/other/utils.h"

#include "src/widgets/colorarea.h"
#include "src/widgets/knowncheckbox.h"
#include "src/widgets/movabletabwidget.h"
#include "src/widgets/pageframe.h"
#include "src/widgets/wordeditframe.h"

#include "src/word/word.h"

// FORMS
extern MainWindow                *mainWindow;

// GENERAL
extern bool                       isAdmin;
extern QString                    currentName;
extern QString                    docPass;
extern QString                    adminPass;

// OTHERS
extern QString                    dir;

extern QString                    protocolCreatorVersion;

#endif // GLOBAL_H
