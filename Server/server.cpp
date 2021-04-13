#include <QDateTime>
#include <QWebSocket>

#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    server = new QWebSocketServer("Server", QWebSocketServer::NonSecureMode, this);
    server->listen(QHostAddress::Any, 8080);
    if (server->isListening())
        qDebug() << QStringLiteral("端口监听成功");

    connect(server, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
}

void Server::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type)

    QString time = QDateTime::currentDateTime().toString(Qt::ISODate);
    auto log = QString("%1: %2 %3\r\n").arg(time).arg(context.function).arg(msg);

    QFile file("server.log");
    file.open(QFile::WriteOnly | QFile::Append);
    file.write(log.toLocal8Bit());
    file.close();

    fprintf(stderr, "%s", log.toLocal8Bit().constData());
}

void Server::onNewConnection()
{
    while (server->hasPendingConnections()) {
        auto socket = new Client(server->nextPendingConnection());
        connect(socket, &Client::disconnected, this, &Server::socketDisconnected);
        sockets.append(socket);
    }
}

void Server::socketDisconnected()
{
    auto socket = qobject_cast<Client *>(sender());

    if (socket) {
        socket->deleteLater();
        sockets.removeAll(socket);
    }
}
