#include "server.h"

#include "../../src/qmllogging.h"

Server::Server(QObject* parent) :
    QTcpServer(parent)
{
}

Server::~Server(void)
{
}

bool Server::start(int port)
{
    if (listen(QHostAddress::Any, port)) {
        m_port = port;
        connect(this, SIGNAL(newConnection()), this, SLOT(handleConnection()));
        LOG(INFO) << "Started server on " << m_port;
        return true;
    }
    LOG(ERROR) << "Unable to start server on " << port << ". Error: " << errorString();
    disconnect(this, SLOT(handleConnection()));
    return false;
}

void Server::handleConnection()
{
    QTcpSocket* conn = nextPendingConnection();
    LOG(INFO) << "Handling connection @" << conn;
}
