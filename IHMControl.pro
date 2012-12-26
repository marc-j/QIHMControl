#-------------------------------------------------
#
# Project created by QtCreator 2012-12-22T21:18:05
#
#-------------------------------------------------

QT       += core gui \
            webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IHMControl
TEMPLATE = app

CONFIG += qt3d

include(libs/3rdparty/qextserialport/src/qextserialport.pri)

SOURCES += main.cpp\
    IHMCore.cpp \
    ui/JoyStickStatus.cpp \
    ui/SystemStatus.cpp \
    ui/IHMControl.cpp \
    ui/MotorStatus.cpp \
    ui/PIDBox.cpp \
    ui/GaugesView.cpp \
    UAV.cpp \
    protocol/Protocol.cpp \
    protocol/message/MessageInterface.cpp \
    ui/SerialConfiguration.cpp \
    ui/Led.cpp \
    ui/Cube3D.cpp \
    ui/UAVView.cpp

HEADERS  += \
    IHMCore.h \
    ui/JoyStickStatus.h \
    ui/SystemStatus.h \
    ui/IHMControl.h \
    ui/MotorStatus.h \
    ui/PIDBox.h \
    ui/GaugesView.h \
    UAV.h \
    protocol/Protocol.h \
    protocol/message/MessageInterface.h \
    ui/SerialConfiguration.h \
    ui/Led.h \
    ui/Cube3D.h \
    ui/UAVView.h

FORMS    += \
    ui/JoyStickStatus.ui \
    ui/SystemStatus.ui \
    ui/IHMControl.ui \
    ui/MotorStatus.ui \
    ui/PIDBox.ui \
    ui/SerialConfiguration.ui

RESOURCES += \
    files.qrc
