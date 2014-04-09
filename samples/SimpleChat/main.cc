#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <QtQml>
#include "../../src/qmllogging.h"

#include "server.h"
#include "client.h"

_INITIALIZE_QMLLOGGING

const int kPort = 15900;

static QObject* singletonClient(QQmlEngine*, QJSEngine*) {
    return new Client();
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    _START_EASYLOGGINGPP(argc, argv);
    
    qmlRegisterSingletonType<Client>("org.easylogging.qml.simplechat", 1, 0, "Messenger", singletonClient);
    el::qml::QMLLogging::registerNew();

    Server server;
    server.start(kPort);
    
    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/SimpleChat/main.qml"));
    viewer.showExpanded();
    
    return app.exec();
}
