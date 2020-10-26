#include <QCoreApplication>
#include <server.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    server.run();

    return a.exec();
}
