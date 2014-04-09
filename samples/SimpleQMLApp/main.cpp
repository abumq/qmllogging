#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "../../src/qmllogging.h"

_INITIALIZE_QMLLOGGING

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    _START_EASYLOGGINGPP(argc, argv);
    
    el::qml::QMLLogging::registerNew("Log");
    
    
    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/QMLLoggingSample/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
