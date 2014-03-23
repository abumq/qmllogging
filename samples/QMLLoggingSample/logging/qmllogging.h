//
//  QMLLogging v1.0 (development version)
//  Single-header only, Easylogging++ extension for QML logging
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
    // Invokable functions
    Q_INVOKABLE void info(QString text) {
        LOG(INFO) << text;
    }
    Q_INVOKABLE void warning(QString text) {
        LOG(WARNING) << text;
    }
    Q_INVOKABLE void debug(QString text) {
        LOG(DEBUG) << text;
    }
    Q_INVOKABLE void error(QString text) {
        LOG(ERROR) << text;
    }
    Q_INVOKABLE void fatal(QString text) {
        LOG(FATAL) << text;
    }
    Q_INVOKABLE void trace(QString text) {
        LOG(TRACE) << text;
    }
    Q_INVOKABLE void verbose(int vlevel, QString text) {
        VLOG(vlevel) << text;
    }
};

#endif // QMLLOGGING_H
