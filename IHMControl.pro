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

CONFIG += opengl qt3d gnuplot
LIBS += -lSDL

include(libs/3rdparty/qextserialport/src/qextserialport.pri)

INCLUDEPATH += src \
    src/ui \
    src/protocol \
    libs/uavlink/include

SOURCES += src/main.cpp\
    src/IHMCore.cpp \
    src/ui/JoyStickStatus.cpp \
    src/ui/SystemStatus.cpp \
    src/ui/IHMControl.cpp \
    src/ui/MotorStatus.cpp \
    src/ui/PIDBox.cpp \
    src/ui/GaugesView.cpp \
    src/UAV.cpp \
    src/protocol/Protocol.cpp \
    src/ui/SerialConfiguration.cpp \
    src/ui/Led.cpp \
    src/ui/Cube3D.cpp \
    src/ui/UAVView.cpp \
    src/ui/JoystickConfiguration.cpp \
    src/input/Joystick.cpp \
    src/ui/JoystickCalibration.cpp \
    src/ui/Console.cpp \
    src/ui/QtPlot/qcustomplot.cpp \
    src/ui/Plot.cpp \
    src/ui/HUD.cpp \
    src/ui/SensorsCalibration.cpp \
    src/ui/SensorsVariance.cpp

HEADERS  += \
    src/IHMCore.h \
    src/ui/JoyStickStatus.h \
    src/ui/SystemStatus.h \
    src/ui/IHMControl.h \
    src/ui/MotorStatus.h \
    src/ui/PIDBox.h \
    src/ui/GaugesView.h \
    src/UAV.h \
    src/protocol/Protocol.h \
    src/ui/SerialConfiguration.h \
    src/ui/Led.h \
    src/ui/Cube3D.h \
    src/ui/UAVView.h \
    src/ui/JoystickConfiguration.h \
    src/input/Joystick.h \
    src/ui/JoystickCalibration.h \
    src/ui/Console.h \
    src/ui/QtPlot/qcustomplot.h \
    src/ui/Plot.h \
    src/ui/HUD.h \
    src/ui/SensorsCalibration.h \
    src/ui/SensorsVariance.h

FORMS    += \
    src/ui/JoyStickStatus.ui \
    src/ui/SystemStatus.ui \
    src/ui/IHMControl.ui \
    src/ui/MotorStatus.ui \
    src/ui/PIDBox.ui \
    src/ui/SerialConfiguration.ui \
    src/ui/JoystickConfiguration.ui \
    src/ui/Console.ui \
    src/ui/SensorsCalibration.ui \
    src/ui/SensorsVariance.ui

RESOURCES += \
    files.qrc
