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
#include <QtCore/QHash>

#define _ELPP_QT_LOGGING
#include <easylogging++.h>

// Invokable log functions
#define FUNCTION_DEFINER(type)\
    Q_INVOKABLE void info(type text) {\
        LOG(INFO) << text;\
    }\
    Q_INVOKABLE void warning(type text) {\
        LOG(WARNING) << text;\
    }\
    Q_INVOKABLE void debug(type text) {\
        LOG(DEBUG) << text;\
    }\
    Q_INVOKABLE void error(type text) {\
        LOG(ERROR) << text;\
    }\
    Q_INVOKABLE void fatal(type text) {\
        LOG(FATAL) << text;\
    }\
    Q_INVOKABLE void trace(type text) {\
        LOG(TRACE) << text;\
    }\
    Q_INVOKABLE void verbose(int vlevel, type text) {\
        VLOG(vlevel) << text;\
    }
namespace el {
namespace qml {
class VersionInfo : el::base::StaticClass {
public:
    static inline int getMajor() { return version()[0].digitValue(); }
    static inline int getMinor() { return version()[2].digitValue(); }
    static inline const QString version(void) { return QString("0.0"); }
    static inline const QString releaseDate(void) { return QString("01-01-2012 0000hrs"); }
};

class TimeTracker {
public:
    void time(QString blockName) {
        m_timedBlocks.insert(blockName, el::base::Trackable(blockName.toStdString(), _ELPP_MIN_UNIT));
    }
    void timeEnd(QString blockName) {
        m_timedBlocks.remove(blockName);
    }
    void timeCheck(QString blockName, QString checkpointId = QString()) {
        QHash<QString, el::base::Trackable>::iterator iterator = m_timedBlocks.find(blockName);
        if (iterator != m_timedBlocks.end()) {
            iterator->checkpoint(checkpointId.toStdString().c_str());
        }
    }
private:
    QHash<QString, el::base::Trackable> m_timedBlocks;
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
private:
    TimeTracker m_tracker;
    QHash<QString, int> m_counters;

    explicit QMLLogging(QObject *parent = 0) : QObject(parent) { }

    static QObject* newInstance(QQmlEngine*, QJSEngine*) {
        return new QMLLogging();
    }
public:
    FUNCTION_DEFINER(QString)
    // Time tracker functions
    Q_INVOKABLE inline void time(QString blockName) {
        m_tracker.time(blockName);
    }
    Q_INVOKABLE inline void timeEnd(QString blockName) {
        m_tracker.timeEnd(blockName);
    }
    Q_INVOKABLE inline void timeCheck(QString blockName, QString checkpointId) {
        m_tracker.timeCheck(blockName, checkpointId);
    }
    // Count functions
    Q_INVOKABLE inline void count(const QString &msg) {
        QHash<QString, int>::iterator iterator = m_counters.find(msg);
        if (iterator == m_counters.end()) {
            iterator = m_counters.insert(msg, 0);
        }
        LOG(INFO) << msg << " {* " << (++*iterator) << "}";
    }
    Q_INVOKABLE inline void countEnd(const QString &msg) {
        m_counters.remove(msg);
    }
};
}  // namespace qml
}  // namespace el
#undef FUNCTION_DEFINER
#endif // QMLLOGGING_H
