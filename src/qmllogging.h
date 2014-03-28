//
//  QMLLogging v0.0 (development / unreleased version)
//  Single-header only, Easylogging++ extension for QML logging
//
//  Requires:
//     * Easylogging++ v9.60 (or higher)
//     * Qt Quick 2.0 (or higher)
//
//  Copyright (c) 2012 - 2014 Majid Khan
//
//  This library is released under the MIT Licence.
//  http://www.easylogging.org/licence.php
//
//  support@easylogging.org
//  http://easylogging.org
//  https://github.com/easylogging/qmllogging
//  https://github.com/easylogging/easyloggingpp
//
#ifndef QMLLOGGING_H
#define QMLLOGGING_H

#include <QtQml>

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QHash>

#define _ELPP_QT_LOGGING
#include <easylogging++.h>

class CallContext;

namespace el {
namespace qml {
class VersionInfo : el::base::StaticClass {
public:
    static inline int getMajor() { return version()[0].digitValue(); }
    static inline int getMinor() { return version()[2].digitValue(); }
    static inline const QString version(void) { return QString("0.0"); }
    static inline const QString releaseDate(void) { return QString("01-01-2012 0000hrs"); }
};

class TimeTracker : el::base::NoCopy {
public:
    typedef QHash<QString, el::base::Trackable*> HashMap;
    
    virtual ~TimeTracker(void) {
        QList<QString>::iterator key = m_timedBlocks.keys().begin();
        for (; key != m_timedBlocks.keys().end(); ++key) {
            delete m_timedBlocks.take(*key);
        }
        m_timedBlocks.clear();
    }
    
    void timeBegin(const QString &blockName) {
        // Why on heap? T is destroyed after insertion, and we don't want this to happen
        // otherwise unnecessary check occurs 
        m_timedBlocks.insert(blockName, 
                             new el::base::Trackable(blockName.toStdString(), _ELPP_MIN_UNIT));
    }
    void timeEnd(const QString &blockName) {
        el::base::Trackable* trackable = m_timedBlocks.take(blockName);
        delete trackable;
    }
    void timeCheck(const QString &blockName, QString checkpointId = QString()) {
        typename HashMap::iterator iterator = m_timedBlocks.find(blockName);
        if (iterator != m_timedBlocks.end()) {
            (*iterator)->checkpoint(checkpointId.toStdString().c_str());
        }
    }
private:
    HashMap m_timedBlocks;
};

class QMLLogging : public QObject
{
    Q_OBJECT
public:
    static void registerNew(const char* contextName = "Log") {
        qmlRegisterSingletonType<QMLLogging>("org.easylogging.qml", 
                                             qml::VersionInfo::getMajor(), qml::VersionInfo::getMinor(),
                                             contextName, QMLLogging::newInstance);
    }
    
    bool hasError(void) const { return m_hasError; }
    QString errorString(void) const { return m_errorString; }
private:
    el::Logger* m_logger;
    TimeTracker m_tracker;
    QHash<QString, int> m_counters;
    bool m_hasError;
    QString m_errorString;
    
    explicit QMLLogging(const char* loggerId = el::base::consts::kDefaultLoggerId,
                        QObject *parent = 0) : QObject(parent),
        m_hasError(false), m_errorString(QString()) {
        m_logger = el::Loggers::getLogger(loggerId);
        if (m_logger == nullptr) {
            m_hasError = true;
            m_errorString = QString("Unable to find or register logger: [" + QString(loggerId) + "]");
        }
    }
    
    static QObject* newInstance(QQmlEngine*, QJSEngine*) {
        return new QMLLogging();
    }
public:
    Q_INVOKABLE void info(const QString &text) {
        if (!m_hasError) m_logger->info(text);
    }
    
    Q_INVOKABLE void warning(const QString &text) {
        if (!m_hasError) m_logger->warn(text);
    }
    
    Q_INVOKABLE void debug(const QString &text) {
        if (!m_hasError) m_logger->debug(text);
    }
    
    Q_INVOKABLE void error(const QString &text) {
        if (!m_hasError) m_logger->error(text);
    }
    
    Q_INVOKABLE void fatal(const QString &text) {\
        if (!m_hasError) m_logger->fatal(text);
    }
    
    Q_INVOKABLE void trace(const QString &text) {
        if (!m_hasError) m_logger->trace(text);
    }
    
    Q_INVOKABLE void verbose(int vlevel, const QString &text) {
        if (!m_hasError) m_logger->verbose(vlevel, text);
    }
    
    // Time tracker functions
    Q_INVOKABLE inline void timeBegin(const QString &blockName) {
        m_tracker.timeBegin(blockName);
    }
    Q_INVOKABLE inline void timeEnd(const QString &blockName) {
        m_tracker.timeEnd(blockName);
    }
    Q_INVOKABLE inline void timeCheck(const QString &blockName, const QString &checkpointId) {
        m_tracker.timeCheck(blockName, checkpointId);
    }
    
    // Count functions
    Q_INVOKABLE inline void count(const QString &msg) {
        if (!m_hasError) {
            QHash<QString, int>::iterator iterator = m_counters.find(msg);
            if (iterator == m_counters.end()) {
                iterator = m_counters.insert(msg, 0);
            }
            m_logger->info("% {%}", msg, ++*iterator);
        }
    }
    
    Q_INVOKABLE inline void countEnd(const QString &msg) {
        m_counters.remove(msg);
    }
};
}  // namespace qml
}  // namespace el
#undef FUNCTION_DEFINER
#endif // QMLLOGGING_H
