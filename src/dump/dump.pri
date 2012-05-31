#-------------------------------------------------
#
# Project created by QtCreator 2012-05-04T17:08:42
#
#-------------------------------------------------

DUMP_SRC = src/dump

win32-msvc* {
    QMAKE_CXXFLAGS_RELEASE += -Zi
    QMAKE_CFLAGS_RELEASE += -Zi
    QMAKE_LFLAGS_RELEASE += /debug
} else {
    QMAKE_CXXFLAGS_RELEASE = -g
    QMAKE_CFLAGS_RELEASE = -g
    QMAKE_LFLAGS_RELEASE =

    CONFIG (release, debug|release) {
        makerelease.commands += \"$$_PRO_FILE_PWD_/$$DUMP_SRC/mingw_make_release.bat\" \"$(MAKE)\" \"$$OUT_PWD/$(DESTDIR)\" \"$(TARGET)\" \
                                ||\
                                echo \"ERROR!!! Impossible to prepare release version. Please check logs\" 1>&2

        first.depends = $(first) makerelease
        makerelease.depends = release

        QMAKE_EXTRA_TARGETS += first makerelease
    }
}

SOURCES += $$DUMP_SRC/dumpcrashes.cpp \
           $$DUMP_SRC/crashdialog.cpp \
           $$DUMP_SRC/exceptionobject.cpp

HEADERS  += $$DUMP_SRC/dumpcrashes.h \
            $$DUMP_SRC/crashdialog.h \
            $$DUMP_SRC/exceptionobject.h

FORMS    += $$DUMP_SRC/crashdialog.ui
