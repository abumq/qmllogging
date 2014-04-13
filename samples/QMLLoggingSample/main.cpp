#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"

#include "../../src/qmllogging.h"

_INITIALIZE_QMLLOGGING

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    _START_EASYLOGGINGPP(argc, argv);
    
    
    QtQuick2ApplicationViewer viewer;
    
    el::qml::QmlLogging::registerNew(viewer.rootContext());
    
    viewer.setMainQmlFile(QStringLiteral("qml/QMLLoggingSample/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
