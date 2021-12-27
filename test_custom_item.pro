QT += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
DESTDIR = ../bin

SOURCES += \
        src/main.cpp \
        src/main_window.cpp

HEADERS += \
        inc/main_window.h

FORMS += \
        forms/main_window.ui

INCLUDEPATH += \
        inc \
        $$PWD/../custom_item/inc

LIBS *= -L$$OUT_PWD/$$DESTDIR -lcustom_item

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
