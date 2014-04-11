#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QThread>

class QTcpSocket;

class ConnectionHandler : public QThread 
{
    Q_OBJECT
public:
    explicit ConnectionHandler(QTcpSocket* tcpSocket, QObject *parent = 0);
protected:
    virtual void run(void);
private:
    QTcpSocket *m_tcpSocket;
signals:
    void ready(QString str);
public slots:
    void onDisconnected(void);
    void onReady(void);
};

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject* parent = 0);
    virtual ~Server(void);
    Q_INVOKABLE bool start(int port);
    Q_INVOKABLE QString info() const;
    int port() const { return m_port; }
public slots:
    void handleConnection();
signals:
    void ready(QString msg);
private:
    int m_port;
    
    QString getIp() const;
};

#endif // SERVER_H
