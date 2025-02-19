#include <QCoreApplication>

#include "logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Logger* logger = new Logger;
    logger->LOG_INFO("first log");
    logger->LOG_INFO("seconds log");

    return a.exec();
}
