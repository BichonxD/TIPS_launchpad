#-------------------------------------------------
#
# Project created by QtCreator 2013-10-20T17:45:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Launchpad
TEMPLATE = app


SOURCES += \
    main.cpp\
    mainwindow.cpp \
    sound_mgr.cpp

HEADERS  += \
    mainwindow.h \
    sound_mgr.h

FORMS    += mainwindow.ui

win32-msvc*:contains(QMAKE_TARGET.arch, x86_64):{
    message( "Building for 64 bit")
    win32:CONFIG(release, debug|release): LIBS += -L"$$PWD/../fmod_lib/" -lfmodex64_vc
    win32:CONFIG(debug, debug|release): LIBS += -L"$$PWD/../fmod_lib/" -lfmodexL64_vc
}
else:win32-msvc*:!contains(QMAKE_TARGET.arch, x86_64):{
    message( "Building for 32 bit")
    win32:CONFIG(release, debug|release): LIBS += -L"$$PWD/../fmod_lib/" -lfmodex_vc
    win32:CONFIG(debug, debug|release): LIBS += -L"$$PWD/../fmod_lib/" -lfmodexL_vc
}

INCLUDEPATH += "$$PWD/../fmod_lib/inc"
DEPENDPATH += "$$PWD/../fmod_lib/inc"
