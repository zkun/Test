#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

#include "client.h"

Client::Client(QWebSocket *parent)
    : QObject(parent),
      socket(parent)
{
    qDebug() << QStringLiteral("新连接") << socket->peerAddress().toString();
    connect(socket, &QWebSocket::disconnected, this, &Client::disconnected);
    connect(socket, &QWebSocket::textMessageReceived, this, &Client::onMessageReceived);

    auto timer = new QTimer(this);
    timer->start(1000*10);

    connect(timer, &QTimer::timeout, [=]() {
        socket->sendTextMessage(QStringLiteral("{\"q\":\"temp\"}"));
    });
}

Client::~Client()
{
    socket->close();
    socket->deleteLater();
}

void Client::onMessageReceived(QString mess)
{
    qDebug() << mess;

    auto obj = QJsonDocument::fromJson(mess.toLocal8Bit()).object();
    if (obj.contains("temp")) {
        m_temp = obj.value("temp").toDouble();

        emit tempChanged(m_temp);
    }
}
