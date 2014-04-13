//
//  QmlLogging v1.3
//  Single-header only, Easylogging++ wrapper for advanced 
//  logging support for QML applications
//
//  Requires:
//     * Easylogging++ v9.68 (or higher)
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
#if !defined(_QMLLOGGING_AVOID_QDEBUG)
#   include <QDebug>
#   define _ELPP_INTERNAL_DEBUGGING_OUT_INFO qDebug()
#   define _ELPP_INTERNAL_DEBUGGING_OUT_ERROR qDebug()
#   define _ELPP_INTERNAL_DEBUGGING_ENDL ""
#   define _ELPP_INTERNAL_DEBUGGING_MSG(msg) QString::fromStdString(msg)
#   define ELPP_CUSTOM_COUT qDebug()
#   if defined(_ELPP_UNICODE)
#   define ELPP_CUSTOM_COUT_LINE(msg) QString::fromWCharArray(msg.c_str()).trimmed()
#   else
#   define ELPP_CUSTOM_COUT_LINE(msg) QString::fromStdString(msg).trimmed()
#   endif  // defined(_ELPP_UNICODE)
#endif  // !defined(_QMLLOGGING_AVOID_QDEBUG)

// NOTE: Include easylogging++ according to your configurations
#include <easylogging++.h>

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
    static inline const QString version(void) { return QString("1.3"); }
    static inline const QString releaseDate(void) { return QString("13-04-2014 2135hrs"); }
};

class TimeTracker : el::base::NoCopy {
public:
    typedef el::base::PerformanceTracker Tracker;
    typedef QHash<QString, QSharedPointer<Tracker>> HashMap;
    
    virtual ~TimeTracker(void) {
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
        m_timedBlocks.insert(blockName, 
            QSharedPointer<Tracker>(new Tracker(blockName.toStdString(), _ELPP_MIN_UNIT, m_loggerId)));
    }
    void timeEnd(const HashMap::key_type& blockName) {
        if (m_timedBlocks.contains(blockName)) {
            m_timedBlocks.remove(blockName);
        }
    }
    void timeCheck(const HashMap::key_type& blockName, QString checkpointId = QString()) {
        HashMap::iterator iterator = m_timedBlocks.find(blockName);
        if (iterator != m_timedBlocks.end()) {
            (*iterator)->checkpoint(checkpointId.toStdString().c_str());
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
    static void registerNew(QQmlContext* rootContext, const char* loggerId = "qml") {
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
        el::Loggers::addFlag(LoggingFlag::DisableApplicationAbortOnFatalLog);
        setObjectName("QmlLogging");
        el::Loggers::getLogger("QmlLogging");
        CLOG_AFTER_N(1, WARNING, "QmlLogging") 
            << "Multiple instances of QmlLogging registered";
        m_loggerId = qml::s_defaultLoggerId;
        el::Loggers::getLogger(m_loggerId);
        m_tracker.setLoggerId(m_loggerId);
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
            CLOG(FATAL, m_loggerId.c_str()) << "Assertion failed: [" << msg << "] ";
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
