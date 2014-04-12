cache()

folder_01.source = qml/SimpleChat
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

TEMPLATE = app
TARGET = SimpleChat

QT       += qml quick gui core network

DEFINES += _ELPP_THREAD_SAFE

QMAKE_CXXFLAGS += -std=c++11

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    server.cc \
    client.cc \
    main.cc

HEADERS += \
    server.h \
    client.h \
    ../../src/qmllogging.h

OTHER_FILES += qml/SimpleChat/main.qml


android: ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

# Installation path
# target.path =

    
# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()
