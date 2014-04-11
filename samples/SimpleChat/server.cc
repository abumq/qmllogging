#include "server.h"

#include "../../src/qmllogging.h"

ConnectionHandler::ConnectionHandler(QTcpSocket *tcpSocket, QObject *parent)
    : QThread(parent), m_tcpSocket(tcpSocket)
{
    
}

void ConnectionHandler::run()
{
    if (m_tcpSocket == nullptr) {
        onDisconnected();
    }
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onReady()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    exec();
}

void ConnectionHandler::onDisconnected() 
{
    if (m_tcpSocket != nullptr) {
        m_tcpSocket->deleteLater();
    }
    exit(); // Exit thread
}

void ConnectionHandler::onReady()
{
    emit ready(QString(m_tcpSocket->readAll()).trimmed());
}

Server::Server(QObject* parent) :
    QTcpServer(parent)
{
}

Server::~Server(void)
{
}

bool Server::start(int port)
{
    if (isListening()) {
        close();
    }
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

QString Server::info() const
{
    return getIp().split(",")[0] + ":" + QString::number(port());
}

void Server::handleConnection()
{
    QTcpSocket* conn = nextPendingConnection();
    LOG(INFO) << "Handling connection @" << conn << " from " <<
        conn->peerAddress().toString();
    ConnectionHandler* handler = new ConnectionHandler(conn, this);
    connect(handler, SIGNAL(ready(QString)), this, SIGNAL(ready(QString)), Qt::DirectConnection);
    connect(handler, SIGNAL(finished()), handler, SLOT(deleteLater()));
    handler->start();    
}

QString Server::getIp() const
{   
    QStringList ips;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
    if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
         ips << address.toString();
    }
    return ips.join(",");
}
