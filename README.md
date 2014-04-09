QML Logging
===========

Easylogging++ wrapper for QML logging

                                       ‫بسم الله الرَّحْمَنِ الرَّحِيمِ


> **Manual For v1.1**

### Quick Links

  [![download] Download Latest](http://qml.easylogging.org/latest.zip)
  
  [![www] Project Homepage](http://qml.easylogging.org/)

  [![pledgie]](http://www.pledgie.com/campaigns/22070)

  [![paypal]](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=4W7YDRCXWURWG)

# Introduction
Provides functionalities to write logs and do various other functions for your QML applications. You should check out [Easylogging++](https://github.com/easylogging/easyloggingpp/) to see some more exciting things you can do.

This project is in early stages and any type of support is accepted.

# Getting Started
### Download
Current stable release is always http://qml.easylogging.org/latest.zip

Before you start, please read basics of Easylogging++, because this project is essentially a wrapper around the library, hence basic functionality is the same.

### Quick Start
In order to get started with QML Logging, you can follow three easy steps;
* Download latest header (both Easylogging++ and QML Logging)
* Include into your project
* Initialize using single macro... and off you go!

```c++
#include "qmllogging.h"

_INITIALIZE_QMLLOGGING

int main(int argv, char* argc[]) {
   ...
   el::qml::QMLLogging::registerNew("Log");
   ...

   // If you wish to access the registered instance of pointer you can do so by
   const el::qml::QMLLogging* instancePtr = el::qml::QMLLogging::getInstancePointer();
   // NOTE: DO NOT DELETE THIS POINTER!
}
```

```c++
import org.easylogging.qml.1.1

...
Log.info("I am info log")
...
```

Please see `samples/` directory for more samples

# API

Here is list of public functions

 * `info(msg, ...)`
 * `warn(msg, ...)`
 * `debug(msg, ...)`
 * `error(msg, ...)`
 * `fatal(msg, ...)`
 * `trace(msg, ...)`
 * `verbose(level, msg, ...)`

 * `timeBegin(blockName)`
 * `timeEnd(blockName)`
 * `timeCheck(blockName)`

 * `count(msg)`
 * `countEnd(msg)`

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

