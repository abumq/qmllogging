//
//  QmlLogging v1.1 (development / unreleased version)
//  Single-header only, Easylogging++ wrapper for QML logging
//
//  Requires:
//     * Easylogging++ v9.67 (or higher)
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

#include <QSharedPointer>
#include <QQuickItem>
#include <QQmlContext>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QHash>

#define _ELPP_QT_LOGGING
// NOTE: Include easylogging++ this according to your configurations
#include "/home/majid/projects/easylogging/easyloggingpp/src/easylogging++.h"

namespace el {
namespace qml {
#define LogStrT QString
#define LogT const LogStrT&

#define FUNCTION_DEFINER(LEVEL, FN_NAME)\
    Q_INVOKABLE void FN_NAME(LogT t, LogT t2 = LogStrT(), LogT t3 = LogStrT(), LogT t4 = LogStrT(),\
    LogT t5 = LogStrT(), LogT t6 = LogStrT(), LogT t7 = LogStrT(), LogT t8 = LogStrT(),\
    LogT t9 = LogStrT(), LogT t10 = LogStrT(), LogT t11 = LogStrT(), LogT t12 = LogStrT()) {\
    if (!m_hasError) { CLOG(LEVEL, m_loggerId.c_str()) <<  t << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 << t11 << t12;}}
#define FUNCTION_DEFINER_V(FN_NAME)\
    Q_INVOKABLE void FN_NAME(int vlevel, LogT t, LogT t2 = LogStrT(), LogT t3 = LogStrT(), LogT t4 = LogStrT(),\
    LogT t5 = LogStrT(), LogT t6 = LogStrT(), LogT t7 = LogStrT(), LogT t8 = LogStrT(),\
    LogT t9 = LogStrT(), LogT t10 = LogStrT(), LogT t11 = LogStrT(), LogT t12 = LogStrT()) {\
    if (!m_hasError) { CVLOG(vlevel, m_loggerId.c_str()) <<  t << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 << t11 << t12;}}
    
class VersionInfo : el::base::StaticClass {
public:
    static inline int getMajor() { return version()[0].digitValue(); }
    static inline int getMinor() { return version()[2].digitValue(); }
    static inline const QString version(void) { return QString("1.1"); }
    static inline const QString releaseDate(void) { return QString("06-04-2014 1700hrs"); }
};

class TimeTracker : el::base::NoCopy {
public:
    typedef el::base::PerformanceTracker Tracker;
    typedef QHash<QString, Tracker> HashMap;
    
    virtual ~TimeTracker(void) {
        /*for (const HashMap::key_type& key : m_timedBlocks.keys()) {
            Tracker* trackable = m_timedBlocks.take(key);
            el::base::utils::safeDelete(trackable);
        }*/
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
        bool removeFlagLaterDispatch = el::Loggers::hasFlag(LoggingFlag::DisablePerformanceTrackingDispatch);
        bool addFlagLaterCallback = el::Loggers::hasFlag(LoggingFlag::PerformanceTrackingCallback);
        if (!removeFlagLaterDispatch) {
            el::Loggers::addFlag(LoggingFlag::DisablePerformanceTrackingDispatch);
        }
        if (addFlagLaterCallback) {
            el::Loggers::removeFlag(LoggingFlag::PerformanceTrackingCallback);
        }
        m_timedBlocks.insert(blockName, 
                             Tracker(blockName.toStdString(), _ELPP_MIN_UNIT,
                                         m_loggerId));
        if (!removeFlagLaterDispatch) {
            el::Loggers::removeFlag(LoggingFlag::DisablePerformanceTrackingDispatch);
        }
        if (addFlagLaterCallback) {
            el::Loggers::addFlag(LoggingFlag::PerformanceTrackingCallback);
        }
    }
    void timeEnd(const HashMap::key_type& blockName) {
        if (m_timedBlocks.contains(blockName)) {
            m_timedBlocks.remove(blockName);
        }
    }
    void timeCheck(const HashMap::key_type& blockName, QString checkpointId = QString()) {
        HashMap::iterator iterator = m_timedBlocks.find(blockName);
        if (iterator != m_timedBlocks.end()) {
            iterator->checkpoint(checkpointId.toStdString().c_str());
        }
    }
private:
    std::string m_loggerId;
    HashMap m_timedBlocks;
};

class QmlLogging;

extern QSharedPointer<QmlLogging> s_qmlLogging;
extern std::string s_defaultLoggerId;

class QmlLogging : public QQuickItem
{
    Q_OBJECT
public:
    static void registerNew(
            QQmlContext* rootContext,
            const char* loggerId = "qml") {
        qml::s_defaultLoggerId = std::string(loggerId);
        qml::s_qmlLogging = QSharedPointer<QmlLogging>(new QmlLogging);
        qmlRegisterType<QmlLogging>("org.easylogging.qml",
                                    qml::VersionInfo::getMajor(), 
                                    qml::VersionInfo::getMinor(),
                                    "QmlLogging");
        if (rootContext != nullptr) {
            rootContext->setContextProperty("log",
                static_cast<QObject*>(qml::s_qmlLogging.data()));
        }
    }
    
    QmlLogging(QQuickItem *parent = 0) : QQuickItem(parent),
            m_hasError(false), m_errorString(QString()) {
        el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
        m_loggerId = qml::s_defaultLoggerId;
        el::Loggers::getLogger(m_loggerId);
        m_tracker.setLoggerId(m_loggerId);
        setObjectName("QmlLogging");
        el::Loggers::getLogger("QmlLogging");
        CLOG_AFTER_N(1, WARNING, "QmlLogging") 
            << "Multiple instances of QmlLogging registered";
    }
    
    bool hasError(void) const { return m_hasError; }
    QString errorString(void) const { return m_errorString; }
private:
    std::string m_loggerId;
    bool m_hasError;
    QString m_errorString;
    TimeTracker m_tracker;
    QHash<QString, int> m_counters;
public:
    
    FUNCTION_DEFINER(INFO, info)
    FUNCTION_DEFINER(WARNING, warn)
    FUNCTION_DEFINER(DEBUG, debug)
    FUNCTION_DEFINER(ERROR, error)
    FUNCTION_DEFINER(FATAL, fatal)
    FUNCTION_DEFINER(TRACE, trace)
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
            CLOG(INFO, m_loggerId.c_str()) << msg << " {" << ++*iterator << "}";
        }
    }
    
    Q_INVOKABLE inline void countEnd(const QString& msg) {
        m_counters.remove(msg);
    }
    
    // Assertion functions
    Q_INVOKABLE inline void assert(bool condition, const QString& msg) {
        if (_ELPP_DEBUG_LOG && !condition) {
            CLOG(FATAL, m_loggerId.c_str()) << "Check failed: [" << msg << "] ";
        }
    }
};
}  // namespace qml
}  // namespace el
#undef FUNCTION_DEFINER
#undef LogStrT
#undef LogT
#undef _INITIALIZE_QMLLOGGING
#define _INITIALIZE_QMLLOGGING \
    _INITIALIZE_EASYLOGGINGPP \
    namespace el { \
        namespace qml { \
            QSharedPointer<QmlLogging> s_qmlLogging; \
            std::string s_defaultLoggerId; \
        } \
    }
#endif // QMLLOGGING_H
