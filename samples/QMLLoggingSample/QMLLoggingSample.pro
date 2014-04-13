# Add more folders to ship with the application, here
folder_01.source = qml/QMLLoggingSample
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QMAKE_CXXFLAGS += -std=c++11
DEFINES += _ELPP_ENABLE_ERRORS


SOURCES += main.cpp

HEADERS += \
    ../../src/qmllogging.h

android: {
    # ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

#DEFINES += _ELPP_DEBUG_ERRORS _ELPP_DEBUG_INFO
# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

qmldir.files = qmldir

OTHER_FILES += \
    MyCircle.qml
