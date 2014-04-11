#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
class QTcpSocket;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject* parent = 0);
    virtual ~Client(void);
    Q_INVOKABLE void sendMessage(const QString&);
    Q_INVOKABLE void sendMessage(const QString&, int, const QString&);
private:
    QTcpSocket* m_tcpSocket;
};

#endif // CLIENT_H
