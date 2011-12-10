#ifndef GLOBAL_H
#define GLOBAL_H

#include "qdebug.h"

#include "src/main/mainwindow.h"
#include "src/main/passworddialog.h"

#include "src/other/encoder.h"
#include "src/other/utils.h"

#include "src/widgets/knowncheckbox.h"
#include "src/widgets/pageframe.h"

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
