TEMPLATE = app

QT += qml quick multimedia
CONFIG += c++11

SOURCES += source/game.cpp \
    source/main.cpp

RESOURCES += qml/qrc.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    source/game.h \
    source/item.h

