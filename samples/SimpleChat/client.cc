#include "client.h"
#include <QTcpSocket>

#include "../../src/qmllogging.h"

Client::Client(QObject *parent) :
    m_tcpSocket(new QTcpSocket(parent))
{
}

Client::~Client(void)
{
    delete m_tcpSocket;
    m_tcpSocket = nullptr;
}

void Client::sendMessage(const QString& msg)
{
    sendMessage("localhost", 15910, msg);
}

void Client::sendMessage(const QString& host, int port, const QString& msg)
{
    if (m_tcpSocket->isOpen()) {
        m_tcpSocket->waitForDisconnected(10000);
    }
    m_tcpSocket->open(QAbstractSocket::ReadWrite);
    m_tcpSocket->connectToHost(host, port);
    LOG(INFO) << "Estabilishing connection to send msg: " << msg;
    m_tcpSocket->waitForConnected(10000);
    if (m_tcpSocket->state() == QTcpSocket::ConnectedState) {
        LOG(INFO) << "Sending message...";
        m_tcpSocket->write(msg.toStdString().c_str());
    } else {
        LOG(ERROR) << "Unable to connect to " << host << ":" << port << ". " << m_tcpSocket->errorString();
    }
    m_tcpSocket->close();
}
