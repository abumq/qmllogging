#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "logging/qmllogging.h"

_INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    _START_EASYLOGGINGPP(argc, argv);
    
    QMLLogging::registerNew();

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/QMLLoggingSample/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
