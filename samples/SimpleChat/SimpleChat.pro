# Add more folders to ship with the application, here
folder_01.source = qml/SimpleChat
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

QT       += core network

QMAKE_CXXFLAGS += -std=c++11

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    server.cc \
    client.cc \
    main.cc

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    server.h \
    client.h \
    ../../src/qmllogging.h
