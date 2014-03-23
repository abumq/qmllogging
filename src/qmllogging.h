//
//  QMLLogging v0.0 (development / unreleased version)
//  Single-header only, Easylogging++ extension for QML logging
//
//  Requires: Easylogging++ v9.59 (or higher)
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

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtQml>

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
class QMLLogging : public QObject
{
    Q_OBJECT
public:
    static void registerNew(const char* contextName = "Log") {
        qmlRegisterSingletonType<QMLLogging>("org.easylogging.qml", 1, 0,
            contextName, QMLLogging::newInstance);
    }
private:
    explicit QMLLogging(QObject *parent = 0) : QObject(parent) { }

    static QObject* newInstance(QQmlEngine*, QJSEngine*) {
        return new QMLLogging();
    }
public:
    FUNCTION_DEFINER(QString)
};
class VersionInfo : el::base::StaticClass {
public:
    /// @brief Current version number
    static inline const std::string version(void) { return std::string("0.0"); }
    /// @brief Release date of current version
    static inline const std::string releaseDate(void) { return std::string("00-00-0000 0000hrs"); }
};
}  // namespace qml
}  // namespace el
#undef FUNCTION_DEFINER
#endif // QMLLOGGING_H
