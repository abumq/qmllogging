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
private:
    QTcpSocket* m_tcpSocket;
    void sendMessage(const QString&, int, const QString&);
};

#endif // CLIENT_H
