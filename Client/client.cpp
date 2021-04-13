#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

#include <windows.h>

#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QWebSocket("Client", QWebSocketProtocol::VersionLatest, this);
    connect(socket, &QWebSocket::stateChanged, [](QAbstractSocket::SocketState state) {
        qDebug() << state;
    });
    connect(socket, &QWebSocket::disconnected, this, &Client::onDisconnected);
    connect(socket, &QWebSocket::textMessageReceived, this, &Client::onMessageReceived);

    QTimer::singleShot(0, this, &Client::connectServer);
}

Client::~Client()
{
}

void Client::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type)
    Q_UNUSED(context)

    QString time = QDateTime::currentDateTime().toString(Qt::ISODate);
    auto log = QString("%1: %2 %3\r\n").arg(time).arg(context.function).arg(msg);

    static QFile file("client.log");
    file.open(QFile::WriteOnly | QFile::Append);
    file.write(log.toLocal8Bit());
    file.close();

    fprintf(stderr, "%s\n", log.toLocal8Bit().constData());
}

void Client::connectServer()
{
    socket->open(QUrl("ws://127.0.0.1:8080"));
}

//断开5后重连
void Client::onDisconnected()
{
    if (socket->state() == QAbstractSocket::UnconnectedState) {
        qDebug() << QStringLiteral("连接断开");

        QTimer::singleShot(1000*5, this, &Client::connectServer);
    }
}

void Client::onMessageReceived(QString mess)
{
    qDebug() << mess;

    auto obj = QJsonDocument::fromJson(mess.toLocal8Bit()).object();
    auto q = obj.value("q").toString();

    if (q.contains(QStringLiteral("temp"))) {
        auto temp = getCPU();
        QJsonObject ret;
        ret.insert("temp", temp);

        socket->sendTextMessage(QJsonDocument(ret).toJson(QJsonDocument::Compact));
    }
}

/**
  CPU温度读取需要高权限，使用可用内存大小代替回传
**/
double Client::getCPU()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx(&statex);

    auto a = statex.ullAvailPhys / (1024 * 1024);
    return a / 1000.0;
}
