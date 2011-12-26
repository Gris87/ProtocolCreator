#ifndef GLOBAL_H
#define GLOBAL_H

#include "qdebug.h"

#include "src/column_types/columntype.h"
#include "src/column_types/integercolumn.h"
#include "src/column_types/stringcolumn.h"
#include "src/column_types/boolcolumn.h"
#include "src/column_types/datecolumn.h"
#include "src/column_types/timecolumn.h"

#include "src/components/pagecomponent.h"
#include "src/components/variableintegerframe.h"
#include "src/components/variablestringframe.h"
#include "src/components/variableboolframe.h"
#include "src/components/variabledateframe.h"
#include "src/components/variabletimeframe.h"
#include "src/components/variablelistframe.h"
#include "src/components/variableextendedlistframe.h"
#include "src/components/variableexpressionframe.h"

#include "src/components/componenttextframe.h"

#include "src/delegates/datedelegate.h"
#include "src/delegates/doubledelegate.h"
#include "src/delegates/listdelegate.h"
#include "src/delegates/timedelegate.h"

#include "src/main/columneditdialog.h"
#include "src/main/columnsizedialog.h"
#include "src/main/functiondialog.h"
#include "src/main/globaldialog.h"
#include "src/main/mainwindow.h"
#include "src/main/pageselectiondialog.h"
#include "src/main/passworddialog.h"
#include "src/main/sectioncontroldialog.h"
#include "src/main/tableeditdialog.h"

#include "src/other/encoder.h"
#include "src/other/types.h"
#include "src/other/utils.h"

#include "src/widgets/colorarea.h"
#include "src/widgets/knowncheckbox.h"
#include "src/widgets/movabletabwidget.h"
#include "src/widgets/pageframe.h"
#include "src/widgets/wordeditframe.h"

#include "src/word/word.h"

// FORMS
extern MainWindow                *mainWindow;
extern GlobalDialog              *globalDialog;

// GENERAL
extern bool                       isAdmin;
extern QString                    currentName;
extern QString                    docPass;
extern QString                    adminPass;

// OTHERS
extern QString                    dir;
extern QStringList                functionsList;
extern double                     pixelToSantimeter;

extern QString                    protocolCreatorVersion;

#endif // GLOBAL_H
