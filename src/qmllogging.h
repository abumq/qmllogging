//
//  QMLLogging v1.0 (development / unreleased version)
//  Single-header only, Easylogging++ wrapper for QML logging
//
//  Requires:
//     * Easylogging++ v9.64 (or higher)
//     * Qt Quick 2.0 (or higher)
//
//  Copyright (c) 2014 Majid Khan
//
//  This library is released under the MIT Licence.
//  http://www.easylogging.org/licence.php
//
//  support@easylogging.org
//  http://qml.easylogging.org
//  https://github.com/easylogging/qmllogging
//
#ifndef QMLLOGGING_H
#define QMLLOGGING_H

#include <QtQml>
#include <QQmlDebuggingEnabler>

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QHash>

#define _ELPP_QT_LOGGING
#include <easylogging++.h>

namespace el {
namespace qml {
#define LogStrT QString
#define LogT const LogStrT&

#define FUNCTION_DEFINER(FN_NAME)\
Q_INVOKABLE void FN_NAME(LogT t, LogT t2 = LogStrT(), LogT t3 = LogStrT(), LogT t4 = LogStrT(),\
        LogT t5 = LogStrT(), LogT t6 = LogStrT(), LogT t7 = LogStrT(), LogT t8 = LogStrT(),\
        LogT t9 = LogStrT(), LogT t10 = LogStrT(), LogT t11 = LogStrT(), LogT t12 = LogStrT()) {\
        if (!m_hasError) m_logger->FN_NAME("%v%v%v%v%v%v%v%v%v%v%v%v", t, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);}
#define FUNCTION_DEFINER_V(FN_NAME)\
Q_INVOKABLE void FN_NAME(int vlevel, LogT t, LogT t2 = LogStrT(), LogT t3 = LogStrT(), LogT t4 = LogStrT(),\
        LogT t5 = LogStrT(), LogT t6 = LogStrT(), LogT t7 = LogStrT(), LogT t8 = LogStrT(),\
        LogT t9 = LogStrT(), LogT t10 = LogStrT(), LogT t11 = LogStrT(), LogT t12 = LogStrT()) {\
        if (!m_hasError) m_logger->verbose(vlevel, "%v%v%v%v%v%v%v%v%v%v%v%v", t, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);}\

class VersionInfo : el::base::StaticClass {
public:
    static inline int getMajor() { return version()[0].digitValue(); }
    static inline int getMinor() { return version()[2].digitValue(); }
    static inline const QString version(void) { return QString("1.0"); }
    static inline const QString releaseDate(void) { return QString("29-03-2014 0052hrs"); }
};

class TimeTracker : el::base::NoCopy {
public:
    typedef el::base::PerformanceTracker Tracker;
    typedef QHash<QString, Tracker*> HashMap;
    
    virtual ~TimeTracker(void) {
        for (const HashMap::key_type& key : m_timedBlocks.keys()) {
            Tracker* trackable = m_timedBlocks.take(key);
            el::base::utils::safeDelete(trackable);
        }
        m_timedBlocks.clear();
    }
    
    void setLoggerId(const std::string& loggerId) {
        m_loggerId = loggerId;
    }

    void timeBegin(const HashMap::key_type& blockName) {
        if (m_loggerId.empty()) {
            ELPP_INTERNAL_ERROR("Set loggerID first!", false);
            return;
        }
        // Why on heap? T is destroyed after insertion, and we don't want this to happen
        // otherwise unnecessary check occurs 
        m_timedBlocks.insert(blockName, 
                             new Tracker(blockName.toStdString(), _ELPP_MIN_UNIT,
                                                        m_loggerId));
    }
    void timeEnd(const HashMap::key_type& blockName) {
        if (m_timedBlocks.contains(blockName)) {
            Tracker* trackable = m_timedBlocks.take(blockName);
            el::base::utils::safeDelete(trackable);
        }
    }
    void timeCheck(const HashMap::key_type& blockName, QString checkpointId = QString()) {
        typename HashMap::iterator iterator = m_timedBlocks.find(blockName);
        if (iterator != m_timedBlocks.end()) {
            (*iterator)->checkpoint(checkpointId.toStdString().c_str());
        }
    }
private:
    std::string m_loggerId;
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
        m_tracker.setLoggerId(loggerId);
        if (m_logger == nullptr) {
            m_hasError = true;
            m_errorString = QString("Unable to find or register logger: [" + QString(loggerId) + "]");
        }
    }
    
    static QObject* newInstance(QQmlEngine*, QJSEngine*) {
        return new QMLLogging();
    }
public:
    
    FUNCTION_DEFINER(info)
    FUNCTION_DEFINER(warn)
    FUNCTION_DEFINER(debug)
    FUNCTION_DEFINER(error)
    FUNCTION_DEFINER(fatal)
    FUNCTION_DEFINER(trace)
    FUNCTION_DEFINER_V(verbose)
    
    // Time tracker functions
    Q_INVOKABLE inline void timeBegin(const QString& blockName) {
        m_tracker.timeBegin(blockName);
    }
    Q_INVOKABLE inline void timeEnd(const QString& blockName) {
        m_tracker.timeEnd(blockName);
    }
    Q_INVOKABLE inline void timeCheck(const QString& blockName, 
            const QString& checkpointId = QString()) {
        m_tracker.timeCheck(blockName, checkpointId);
    }
    
    // Count functions
    Q_INVOKABLE inline void count(const QString& msg) {
        if (!m_hasError) {
            QHash<QString, int>::iterator iterator = m_counters.find(msg);
            if (iterator == m_counters.end()) {
                iterator = m_counters.insert(msg, 0);
            }
            m_logger->info("%v {%v}", msg, ++*iterator);
        }
    }
    
    Q_INVOKABLE inline void countEnd(const QString& msg) {
        m_counters.remove(msg);
    }

    // Assertion functions
    Q_INVOKABLE inline void assert(bool condition, const QString& msg) {
        if (_ELPP_DEBUG_LOG && !condition) {
            m_logger->fatal("Check failed: [%v] ", msg);
        }
    }
};
}  // namespace qml
}  // namespace el
#undef FUNCTION_DEFINER
#undef LogT
#define _INITIALIZE_QMLLOGGING _INITIALIZE_EASYLOGGINGPP
#endif // QMLLOGGING_H
