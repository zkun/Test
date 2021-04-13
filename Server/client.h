#ifndef CLIENT_H
#define CLIENT_H

#include <QWebSocket>

class Client : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double temp READ temp NOTIFY tempChanged)
public:
    explicit Client(QWebSocket *parent);
    ~Client();

    double temp() const
    {
        return m_temp;
    }

signals:
    void disconnected();
    void tempChanged(double temp);

private:
    double m_temp;
    QWebSocket *socket;

private slots:
    void onMessageReceived(QString mess);
};

#endif // CLIENT_H
