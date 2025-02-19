#include "logger.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

Logger::Logger(QObject *parent)
    : QObject{parent}
    , m_logBuffer(&m_logData)
{
    m_startDate = m_startDate.currentDateTimeUtc();
    readConfigurationFile();
    m_logBuffer << m_startDate.toString("dd.MM.yyyy") << ": logger initialized\n";
    m_timer = new QTimer;
    m_logCategory = Seconds;
    m_logInterval = 10;

    switch (m_logCategory) {
    case Seconds:
        // Set timer interval to m_logInterval seconds
        m_timer->setInterval(1000 * m_logInterval);
        m_timer->start(1000 * m_logInterval);
        break;

    case Minutes:
        // Set timer interval to m_logInterval minutes
        m_timer->setInterval(1000 * 60 * m_logInterval);
        m_timer->start(1000 * 60 * m_logInterval);
        break;

    case Hours:
        // Set timer interval to m_logInterval hours
        m_timer->setInterval(1000 * 60 * 60 * m_logInterval);
        m_timer->start(1000 * 60 * 60 * m_logInterval);
        break;

    case Days:
        // Set timer interval to m_logInterval days
        m_timer->setInterval(1000 * 60 * 60 * 24 * m_logInterval);
        m_timer->start(1000 * 60 * 60 * 24 * m_logInterval);
        break;
    default:
        // If no log category configuration found, set it to hours
        m_timer->setInterval(1000 * 60 * 60 * m_logInterval);
        m_timer->start(1000 * 60 * 60 * m_logInterval);
        break;
    }

    connect(m_timer, &QTimer::timeout, this, &Logger::intervalLoggerFunction);
}

Logger::~Logger() {
    // If logger goes out of scope, flush buffer to file.
    intervalLoggerFunction();
}

void Logger::readConfigurationFile() {
    QFile file("/home/wallog/loggerlib/logconfig.json");
    QString val;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        val = file.readAll();
        file.close();

        QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject sett1 = d.object();
        QJsonValue value = sett1.value(QString("logTimer"));
        qDebug() << value;
        QJsonObject item = value.toObject();
        m_logInterval = item["logInterval"].toString().toInt();
        m_logFileDays = item["logMaximum"].toString().toInt();
        qDebug() << m_logInterval;
        if (item["logCategory"].toString().compare("days") == 0) {
            m_logCategory = Days;
        } else if (item["logCategory"].toString().compare("hours") == 0) {
            m_logCategory = Hours;
        } else if (item["logCategory"].toString().compare("minutes") == 0) {
            m_logCategory = Minutes;
        } else if (item["logCategory"].toString().compare("seconds") == 0) {
            m_logCategory = Seconds;
        }
        qDebug() << item["logCategory"].toString();

        m_logBuffer << QDateTime::currentDateTimeUtc().toString("dd.MM.yyyy")<< ": json configuration file read!\n";
    } else {
        qDebug() << "failed to open json file";
    }
}

void Logger::intervalLoggerFunction() {
    if (m_startDate.daysTo(QDateTime::currentDateTimeUtc()) > m_logFileDays) {
        QFile file(m_startDate.toString("dd.MM.yyyy") + "_log.txt");
        file.remove();
        m_startDate = m_startDate.addDays(1);
    }
    if (m_logData.isEmpty()) {
        return;
    }

    QString logFileName = QDateTime::currentDateTimeUtc().toString("dd.MM.yyyy") + "_log.txt";
    QFile logFile(logFileName);

    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream fileStream(&logFile);
        fileStream << m_logData;
        logFile.close();
        m_logData.clear(); // Clear buffer after writing
    } else {
        qDebug() << "Error opening log file:" << logFile.errorString();
    }
}

void Logger::LOG_INFO(const QString& logString) {
    m_logBuffer << QDateTime::currentDateTimeUtc().toString("dd.MM.yyyy hh:mm:ss")
                << ": " << logString << "\n";
}
