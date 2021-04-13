#include <QFile>
#include <QDateTime>
#include <QCoreApplication>

#include "client.h"

int main(int argc, char *argv[])
{
    qInstallMessageHandler(Client::myMessageOutput);

    QCoreApplication a(argc, argv);

    Client c;

    return a.exec();
}
