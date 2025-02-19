#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QDateTime>

enum LogCategory {
    Seconds,
    Minutes,
    Hours,
    Days,
};

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();

    void readConfigurationFile();
    void LOG_INFO(const QString& logString);

public slots:
    void intervalLoggerFunction();

private:
    QFile* m_logFile;
    QTimer* m_timer;
    QTextStream m_logBuffer;
    QString m_logData;
    QString m_dateString;
    LogCategory m_logCategory;
    int m_logInterval;
    QDateTime m_startDate;
    QDateTime m_currentDate;
signals:
};

#endif // LOGGER_H
