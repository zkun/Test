#ifndef SERVER_H
#define SERVER_H

#include <QFile>
#include <QWebSocketServer>

#include "client.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

signals:

private:
    QList<Client*> sockets;
    QWebSocketServer *server;

private slots:
    void onNewConnection();
    void socketDisconnected();
};

#endif // SERVER_H
