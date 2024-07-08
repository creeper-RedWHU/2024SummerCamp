QT       += core gui
QT       += widgets
QT       += charts
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admin.cpp \
    datavisualization.cpp \
    forecast.cpp \
    historyweather.cpp \
    link_py.cpp \
    login_register.cpp \
    main.cpp \
    mainwindow.cpp \
    sectionwidget.cpp \
    todayweather.cpp

HEADERS += \
    admin.h \
    datavisualization.h \
    forecast.h \
    historyweather.h \
    link_py.h \
    login_register.h \
    mainwindow.h \
    sectionwidget.h \
    todayweather.h

FORMS += \
    admin.ui \
    datavisualization.ui \
    forecast.ui \
    historyweather.ui \
    login_register.ui \
    mainwindow.ui \
    sectionwidget.ui \
    todayweather.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc

win32:CONFIG(release, debug|release): LIBS += -LD:/Python/release/ -lpython312
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/Python/debug/ -lpython312

INCLUDEPATH += D:/Python/include
DEPENDPATH += D:/Python/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/Python/release/libpython312.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/Python/debug/libpython312.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/Python/release/python312.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/Python/debug/python312.lib

DISTFILES += \
    Python/demo.py
