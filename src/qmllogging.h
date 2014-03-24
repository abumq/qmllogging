//
//  QMLLogging v0.0 (development / unreleased version)
//  Single-header only, Easylogging++ extension for QML logging
//
//  Requires:
//     * Easylogging++ v9.59 (or higher)
//     * Qt 5.2 (or higher)
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
#include <QtCode/QHash>

#define _ELPP_QT_LOGGING
#include <easylogging++.h>

// Invokable log functions
#define LOG_LEVEL_FUNCTION(type)\
    Q_INVOKABLE inline void info(type text) {\
        LOG(INFO) << text;\
    }\
    Q_INVOKABLE inline void warning(type text) {\
        LOG(WARNING) << text;\
    }\
    Q_INVOKABLE inline void debug(type text) {\
        LOG(DEBUG) << text;\
    }\
    Q_INVOKABLE inline void error(type text) {\
        LOG(ERROR) << text;\
    }\
    Q_INVOKABLE inline void fatal(type text) {\
        LOG(FATAL) << text;\
    }\
    Q_INVOKABLE inline void trace(type text) {\
        LOG(TRACE) << text;\
    }\
    Q_INVOKABLE inline void verbose(int vlevel, type text) {\
        VLOG(vlevel) << text;\
    }
namespace el {
namespace qml {
class VersionInfo : el::base::StaticClass {
public:
    static inline int major() { return version()[0].digitValue(); }
    static inline int minor() { return version()[2].digitValue(); }
    static inline const QString version(void) { return QString("0.0"); }
    static inline const QString releaseDate(void) { return QString("01-01-2012 0000hrs"); }
};

class TimeTracker {
public:
    
private:
    QHash<QString, el::base::Trackable> m_timedBlocks;
};

class QMLLogging : public QObject
{
    Q_OBJECT
public:
    static void registerNew(const char* contextName = "Log") {
        qmlRegisterSingletonType<QMLLogging>("org.easylogging.qml", qml::VersionInfo::major(), qml::VersionInfo::minor(),
            contextName, QMLLogging::newInstance);
    }
private:
    QHash<QString, el::base::Trackable> m_timedBlocks;
    QHash<QString, int> m_counters;

    explicit QMLLogging(QObject *parent = 0) : QObject(parent) { }

    static QObject* newInstance(QQmlEngine*, QJSEngine*) {
        return new QMLLogging();
    }
public:
    LOG_LEVEL_FUNCTION(const QString &)
    // Time tracker functions
    Q_INVOKABLE inline void time(const QString &timerName) {
        QHash<QString, el::base::Trackable>::iterator = m_timedBlocks.find(timerName);
        if (iterator == m_timedBlocks.end()) {
            m_timedBlocks.insert(timerName, el::base::Trackable(timerName.toStdString(), _ELPP_MIN_UNIT));
        }
    }
    Q_INVOKABLE inline void timeEnd(const QString &timerName) {
        QHash<QString, el::base::Trackable>::iterator = m_timedBlocks.find(timerName);
        if (iterator != m_timedBlocks.end()) {
            m_timedBlocks.remove(iterator);
        }
    }
    Q_INVOKABLE inline void timeCheck(const QString &timerName, QString checkpointId) {
        QHash<QString, el::base::Trackable>::iterator = m_timedBlocks.find(timerName);
        if (iterator != m_timedBlocks.end()) {
            iterator->checkpoint(checkpointId.toStdString().c_str());
        }
    }
    // Count functions
    Q_INVOKABLE inline void count(const QString &msg) {
        QHash<QString, int>::iterator = m_counters.find(msg);
        if (iterator == m_counters.end()) {
            m_counters.insert(msg, 0);
        } else {
           (*iterator)++
        }
        LOG(INFO) << msg << " {* " << (*iterator) << "}";
    }
    Q_INVOKABLE inline void countEnd(const QString &msg) {
        QHash<QString, int>::iterator = m_counters.find(msg);
        if (iterator != m_counters.end()) {
            m_counters.remove(iterator);
        }
    }
};
}  // namespace qml
}  // namespace el
#undef LOG_LEVEL_FUNCTION
#endif // QMLLOGGING_H
