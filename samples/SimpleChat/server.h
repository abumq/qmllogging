#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class Server : public QTcpServer
{
public:
    explicit Server(QObject* parent = 0);
    virtual ~Server(void);
    bool start(int port);
    
public slots:
    void handleConnection();
    
private:
    int m_port;
};

#endif // SERVER_H
