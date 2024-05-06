# 声明使用的Qt模块
QT += core gui multimedia

# 如果Qt的主版本号大于4，则添加widgets模块
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 设置使用C++11标准
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
# 定义使得编译器在使用任何已被标记为已弃用的Qt特性时发出警告
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
# 也可以使得代码在使用已弃用的API时编译失败，取消以下行的注释
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 源文件
SOURCES += \
    gamewidget.cpp \
    main.cpp \
    mainwindow.cpp

# 头文件
HEADERS += \
    gamewidget.h \
    mainwindow.h

# UI文件
FORMS += \
    gamewidget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# 资源文件
RESOURCES += \
    icon.qrc \
    sound.qrc

DISTFILES += \
    MainWindow.qml \
    MainWindowForm.ui.qml

OTHER_FILES += myapp.rc

RC_FILE += myapp.rc

TARGET = snake
DESTDIR = $$absolute_path($${_PRO_FILE_PWD_}/release/)
