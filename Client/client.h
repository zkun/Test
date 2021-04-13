#ifndef CLIENT_H
#define CLIENT_H

#include <QFile>
#include <QDateTime>
#include <QWebSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
signals:

private:
    QWebSocket *socket;

    double getCPU();

private slots:
    void connectServer();
    void onDisconnected();
    void onMessageReceived(QString mess);
};

#endif // CLIENT_H
