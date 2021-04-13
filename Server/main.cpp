#include <QCoreApplication>

#include "server.h"

int main(int argc, char *argv[])
{
    qInstallMessageHandler(Server::myMessageOutput);
    QCoreApplication app(argc, argv);

    Server s;

    return app.exec();
}
