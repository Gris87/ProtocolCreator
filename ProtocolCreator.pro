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
    src/widgets/colorarea.cpp \
    src/main/passworddialog.cpp \
    src/components/pagecomponent.cpp \
    src/components/variableintegerframe.cpp \
    src/components/variablestringframe.cpp \
    src/components/variableboolframe.cpp \
    src/components/variabledateframe.cpp \
    src/components/variabletimeframe.cpp \
    src/components/variablelistframe.cpp \
    src/components/componenttextframe.cpp \
    src/widgets/wordeditframe.cpp \
    src/main/sectioncontroldialog.cpp \
    src/widgets/wordtextedit.cpp \
    src/main/globaldialog.cpp \
    src/main/pageselectiondialog.cpp \
    src/components/variableextendedlistframe.cpp \
    src/components/variableexpressionframe.cpp \
    src/main/functiondialog.cpp \
    src/delegates/doubledelegate.cpp \
    src/delegates/datedelegate.cpp \
    src/delegates/timedelegate.cpp \
    src/delegates/listdelegate.cpp \
    src/main/tableeditdialog.cpp \
    src/widgets/copyabletable.cpp \
    src/widgets/cellalignmentwidget.cpp \
    src/main/columnsizedialog.cpp \
    src/widgets/unitedtable.cpp \
    src/main/columneditdialog.cpp \
    src/column_types/columntype.cpp \
    src/column_types/integercolumn.cpp \
    src/column_types/stringcolumn.cpp \
    src/column_types/boolcolumn.cpp \
    src/column_types/datecolumn.cpp \
    src/column_types/timecolumn.cpp \
    src/widgets/datatable.cpp \
    src/delegates/stringdelegate.cpp \
    src/main/fulldialog.cpp \
    src/column_types/listcolumn.cpp \
    src/column_types/extendedlistcolumn.cpp \
    src/column_types/expressioncolumn.cpp \
    src/main/listselectiondialog.cpp \
    src/main/insertlinktabledialog.cpp \
    src/main/searchdialog.cpp \
    src/word/xml/wordxml.cpp \
    src/word/xml/wordxmlproperties.cpp \
    src/word/xml/wordxmlcustomproperties.cpp \
    src/word/xml/wordxmlfonts.cpp \
    src/word/xml/wordxmlstyles.cpp \
    src/word/xml/wordxmlstyle.cpp \
    src/word/xml/wordxmlcustomproperty.cpp \
    src/word/xml/wordxmlrunproperties.cpp \
    src/word/xml/wordxmlparagraphproperties.cpp \
    src/word/xml/wordxmltableproperties.cpp \
    src/word/xml/wordxmltablecellmargin.cpp \
    src/word/xml/wordxmltabs.cpp \
    src/word/xml/wordxmltab.cpp \
    src/word/xml/wordxmldocumentproperties.cpp \
    src/word/xml/wordxmlcompatibility.cpp \
    src/word/xml/wordxmlfootnoteproperties.cpp \
    src/word/xml/wordxmlfootnote.cpp \
    src/word/xml/wordxmlparagraph.cpp \
    src/word/xml/wordxmlendnoteproperties.cpp \
    src/word/xml/wordxmlendnote.cpp \
    src/word/xml/wordxmlbase.cpp \
    src/word/xml/wordxmlrun.cpp \
    src/word/xml/wordxmlseparator.cpp \
    src/word/xml/wordxmltabchar.cpp \
    src/word/xml/wordxmlpagenumber.cpp \
    src/word/xml/wordxmlcontinuationseparator.cpp \
    src/word/xml/wordxmlbreak.cpp \
    src/word/xml/wordxmlsections.cpp \
    src/word/xml/wordxmlsection.cpp \
    src/word/xml/wordxmlsectionproperties.cpp

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
    src/widgets/colorarea.h \
    src/main/passworddialog.h \
    src/components/pagecomponent.h \
    src/components/variableintegerframe.h \
    src/components/variablestringframe.h \
    src/components/variableboolframe.h \
    src/components/variabledateframe.h \
    src/components/variabletimeframe.h \
    src/components/variablelistframe.h \
    src/components/componenttextframe.h \
    src/widgets/wordeditframe.h \
    src/main/sectioncontroldialog.h \
    src/widgets/wordtextedit.h \
    src/main/globaldialog.h \
    src/main/pageselectiondialog.h \
    src/components/variableextendedlistframe.h \
    src/components/variableexpressionframe.h \
    src/main/functiondialog.h \
    src/delegates/doubledelegate.h \
    src/delegates/datedelegate.h \
    src/delegates/timedelegate.h \
    src/delegates/listdelegate.h \
    src/main/tableeditdialog.h \
    src/widgets/copyabletable.h \
    src/widgets/cellalignmentwidget.h \
    src/main/columnsizedialog.h \
    src/widgets/unitedtable.h \
    src/other/types.h \
    src/main/columneditdialog.h \
    src/column_types/columntype.h \
    src/column_types/integercolumn.h \
    src/column_types/stringcolumn.h \
    src/column_types/boolcolumn.h \
    src/column_types/datecolumn.h \
    src/column_types/timecolumn.h \
    src/widgets/datatable.h \
    src/delegates/stringdelegate.h \
    src/main/fulldialog.h \
    src/column_types/listcolumn.h \
    src/column_types/extendedlistcolumn.h \
    src/column_types/expressioncolumn.h \
    src/main/listselectiondialog.h \
    src/main/insertlinktabledialog.h \
    src/main/searchdialog.h \
    src/word/xml/wordxml.h \
    src/word/xml/wordxmlproperties.h \
    src/word/xml/wordxmlcustomproperties.h \
    src/word/xml/wordxmlfonts.h \
    src/word/xml/wordxmlstyles.h \
    src/word/xml/wordxmlstyle.h \
    src/word/xml/wordxmlcustomproperty.h \
    src/word/xml/wordxmlrunproperties.h \
    src/word/xml/wordxmlparagraphproperties.h \
    src/word/xml/wordxmltableproperties.h \
    src/word/xml/wordxmltablecellmargin.h \
    src/word/xml/wordxmltabs.h \
    src/word/xml/wordxmltab.h \
    src/word/xml/wordxmldocumentproperties.h \
    src/word/xml/wordxmlcompatibility.h \
    src/word/xml/wordxmlfootnote.h \
    src/word/xml/wordxmlfootnoteproperties.h \
    src/word/xml/wordxmlparagraph.h \
    src/word/xml/wordxmlendnote.h \
    src/word/xml/wordxmlendnoteproperties.h \
    src/word/xml/wordxmlbase.h \
    src/word/xml/wordxmlrun.h \
    src/word/xml/wordxmltypes.h \
    src/word/xml/wordxmlseparator.h \
    src/word/xml/wordxmlpagenumber.h \
    src/word/xml/wordxmlcontinuationseparator.h \
    src/word/xml/wordxmlbreak.h \
    src/word/xml/wordxmltabchar.h \
    src/word/xml/wordxmlsection.h \
    src/word/xml/wordxmlsections.h \
    src/word/xml/wordxmlsectionproperties.h

FORMS    += src/main/mainwindow.ui \
    src/widgets/pageframe.ui \
    src/main/passworddialog.ui \
    src/components/variableintegerframe.ui \
    src/components/variablestringframe.ui \
    src/components/variableboolframe.ui \
    src/components/variabledateframe.ui \
    src/components/variabletimeframe.ui \
    src/components/variablelistframe.ui \
    src/components/componenttextframe.ui \
    src/widgets/wordeditframe.ui \
    src/main/sectioncontroldialog.ui \
    src/main/globaldialog.ui \
    src/main/pageselectiondialog.ui \
    src/components/variableextendedlistframe.ui \
    src/components/variableexpressionframe.ui \
    src/main/functiondialog.ui \
    src/main/tableeditdialog.ui \
    src/widgets/cellalignmentwidget.ui \
    src/main/columnsizedialog.ui \
    src/main/columneditdialog.ui \
    src/main/fulldialog.ui \
    src/main/listselectiondialog.ui \
    src/main/insertlinktabledialog.ui \
    src/main/searchdialog.ui

OTHER_FILES += \
    TODO.txt
