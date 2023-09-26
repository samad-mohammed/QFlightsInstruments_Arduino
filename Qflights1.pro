QT       += core gui
QT     += svgwidgets serialport location positioning quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#QT += location

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h


FORMS += \
    mainwindow.ui

include(qfi/qfi.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Maps.qrc \
    qml.qrc
