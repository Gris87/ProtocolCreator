#-------------------------------------------------
#
# Project created by QtCreator 2011-11-21T15:12:31
#
#-------------------------------------------------

QT       += core gui

TARGET = ProtocolCreator
TEMPLATE = app

LIBS += -lqaxcontainer

RC_FILE = Resources.rc
RESOURCES += Resources.qrc

SOURCES += src/main.cpp\
        src/main/mainwindow.cpp \
    src/other/global.cpp \
    src/word/word.cpp \
    src/word/wordbase.cpp \
    src/word/wordapp.cpp \
    src/word/worddocuments.cpp \
    src/word/wordworkbase.cpp \
    src/word/worddocument.cpp \
    src/word/wordoptions.cpp \
    src/word/wordrange.cpp \
    src/word/wordtables.cpp \
    src/word/wordtable.cpp \
    src/word/wordtablecell.cpp \
    src/other/utils.cpp \
    src/other/encoder.cpp \
    src/widgets/pageframe.cpp \
    src/widgets/knowncheckbox.cpp \
    src/widgets/movabletabwidget.cpp \
    src/main/passworddialog.cpp \
    src/components/pagecomponent.cpp \
    src/components/variableintegerframe.cpp \
    src/components/variablestringframe.cpp \
    src/components/variableboolframe.cpp

HEADERS  += src/main/mainwindow.h \
    src/other/global.h \
    src/word/word.h \
    src/word/wordbase.h \
    src/word/wordapp.h \
    src/word/worddocuments.h \
    src/word/wordworkbase.h \
    src/word/worddocument.h \
    src/word/wordoptions.h \
    src/word/wordrange.h \
    src/word/wordtables.h \
    src/word/wordtable.h \
    src/word/wordtablecell.h \
    src/other/utils.h \
    src/other/encoder.h \
    src/widgets/pageframe.h \
    src/widgets/knowncheckbox.h \
    src/widgets/movabletabwidget.h \
    src/main/passworddialog.h \
    src/components/pagecomponent.h \
    src/components/variableintegerframe.h \
    src/components/variablestringframe.h \
    src/components/variableboolframe.h

FORMS    += src/main/mainwindow.ui \
    src/widgets/pageframe.ui \
    src/main/passworddialog.ui \
    src/components/variableintegerframe.ui \
    src/components/variablestringframe.ui \
    src/components/variableboolframe.ui
