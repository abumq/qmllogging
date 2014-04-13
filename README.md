QML Logging
===========

Easylogging++ wrapper for advanced logging support for QML applications

                                       ‫بسم الله الرَّحْمَنِ الرَّحِيمِ


> **Manual For v1.1**

### Quick Links

  [![download] Download Latest](http://qml.easylogging.org/latest.zip)
  
  [![notes] Release Notes](https://github.com/easylogging/qmllogging/tree/master/doc/RELEASE-NOTES-v1.1)
 
  [![samples] Samples](https://github.com/easylogging/qmllogging/tree/v1.1/samples)
  
  [![www] Project Homepage](http://qml.easylogging.org/)

  [![pledgie]](http://www.pledgie.com/campaigns/22070)

  [![paypal]](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=4W7YDRCXWURWG)

# Introduction
Provides functionalities to write logs and provides various other functionalities for your QML applications. You should check out [Easylogging++](https://github.com/easylogging/easyloggingpp/) to see more exciting things you can do.

# Getting Started
### Download
Current stable release is http://qml.easylogging.org/latest.zip

Before you start, please read basics of [Easylogging++](https://github.com/easylogging/easyloggingpp/) as this project is essentially a wrapper around the library, hence basic functionality is the same.

### Quick Start
In order to get started with QML Logging, you can follow three easy steps;
* Download latest header (both Easylogging++ and QML Logging)
* Include into your project
* Initialize using single macro... and off you go!

```c++
#include "qmllogging.h"

_INITIALIZE_QMLLOGGING

int main(int argc, char* argv[]) {
   QGuiApplication app(argc, argv);
   // Following line is optional but always recommended to put this - needed by some functionalities
   _START_EASYLOGGINGPP(argc, argv);
   
   // Your Qml engine
   QQmlApplicationEngine engine(QUrl("...));
   
   el::qml::QmlLogging::registerNew(engine.rootContext());
   
   // Or if you are using QtQuick2ApplicationViewer
   // QtQuick2ApplicationViewer viewer;
   // el::qml::QMLLogging::registerNew(viewer.rootContext());
   
   // ...
   
   return app.exec();
}
```

```c++
import org.easylogging.qml 1.1

...
log.info("I am info log")
...
```

### Notes

#### 1. Default logger
Default logger for QML logging is `qml`. You can change this by passing second argument to `registerNew` function.

#### 2. Log to file
By default, your QML application will also write to `logs/` directory, you can disable this behaviour by reconfiguring default configurations (also update existing loggers). This configuration will have `el::ConfigurationType::ToFile` to `false`.

```c++
#include "qmllogging.h"

_INITIALIZE_QMLLOGGING

int main(int argc, char* argv[]) {
   // ... Your code
   _START_EASYLOGGINGPP(argc, argv);

   el::Configurations myConfigurations;
   myConfigurations.setGlobally(ConfigurationType::ToFile, "false");
   Loggers::setDefaultConfigurations(myConfigurations, true);

   el::qml::QmlLogging::registerNew("Log");
   
   // ... Your code
}
```

Please note that some android devices do not allow to create `logs/` directory or log file, please check permissions. And you are facing issues and do not see any log when doing remote debugging (or from `adb shell`), enable error viewing by defining `_ELPP_DEBUG_ERRORS` to see whats the issue.

#### 3. Log to standard output
QML logging uses `std::cout` to log to standard output (i.e, console or terminal) 

 > Please see `samples/` directory for samples

# API

Here is list of public functions

#### Logging
 * `info(msg, ...)`
 * `warn(msg, ...)`
 * `debug(msg, ...)`
 * `error(msg, ...)`
 * `fatal(msg, ...)`
 * `trace(msg, ...)`
 * `verbose(level, msg, ...)`

#### Timing
 * `timeBegin(blockName)`
 * `timeEnd(blockName)`
 * `timeCheck(blockName)`

#### Couting
 * `count(msg)`
 * `countEnd(msg)`

#### Assertion
 * `assert(condition, msg)`

# Licence
```
The MIT License (MIT)

Copyright (c) 2014 Majid Khan
http://easylogging.org/

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
```

  [banner]: http://easylogging.org/images/banner.png?v=4
  [download]: http://easylogging.org/images/download.png?v=2
  [www]: http://easylogging.org/images/logo-www.png?v=2
  [paypal]: https://www.paypalobjects.com/en_AU/i/btn/btn_donateCC_LG.gif
  [pledgie]: https://pledgie.com/campaigns/22070.png
  [samples]: http://easylogging.org/images/sample.png?v=2
  [notes]: http://easylogging.org/images/notes.png?v=4
