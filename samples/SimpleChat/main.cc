#include <QtGui/QGuiApplication>
//#include "qtquick2applicationviewer.h"
#include <QtQml>
#include <QQuickWindow>
#include "../../src/qmllogging.h"

#include "server.h"
#include "client.h"

_INITIALIZE_QMLLOGGING

const int kPort = 15900;

static QObject* singletonClient(QQmlEngine*, QJSEngine*) {
    return new Client();
}

Server* server = new Server();

static QObject* singletonServer(QQmlEngine*, QJSEngine*) {
    return server;
}

const char* getThreadId() {
    return ""; // Custom thread id
}

static el::Configurations defaultLogConfigurations() {
    el::Configurations result;
    result.set(el::Level::Global, el::ConfigurationType::Format,
        "%datetime{%H:%m:%s} %level %logger [%thread] %msg");
    result.setRemainingToDefault();
    return result;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    _START_EASYLOGGINGPP(argc, argv);

    el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    el::Helpers::installCustomFormatSpecifier(el::CustomFormatSpecifier("%tid", getThreadId));
    el::Loggers::setDefaultConfigurations(defaultLogConfigurations(), true);
    
    //bool serverStarted = server->start(kPort);
    //LOG_IF(!serverStarted, INFO) << "Unable to start server";
    
    qmlRegisterSingletonType<Client>("org.easylogging.qml.simplechat", 1, 0, "Messenger", singletonClient);
    qmlRegisterSingletonType<Server>("org.easylogging.qml.simplechat", 1, 0, "Server", singletonServer);
    
    el::qml::QMLLogging::registerNew();
   
    QQmlApplicationEngine viewer(QUrl("qml/SimpleChat/main.qml"));
    QObject *topLevel = viewer.rootObjects().value(0);

    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
    if (!window) {
        LOG(WARNING) << "Error: Your root item has to be a Window.";
        return -1;
    }
    QString title = "SimpleChat - ";
    if (server->isListening()) {
        title += "running:" + QString::number(server->port());
    } else {
        title += "idle";
    }
    window->setTitle(title);
    window->show();

    ((QQmlContext*)viewer.rootContext())->setContextProperty("serverObj", server);
    
    return app.exec();
}
