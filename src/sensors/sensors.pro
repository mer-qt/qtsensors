load(qt_build_config)

TARGET = QtSensors
CONFIG += strict_flags
QT = core-private

CONFIG(debug,debug|release):DEFINES += ENABLE_RUNTIME_SENSORLOG
!isEmpty(SENSORS_CONFIG_PATH):DEFINES += "QTSENSORS_CONFIG_PATH=\\\"$$SENSORS_CONFIG_PATH\\\""

simulator {
    DEFINES += SIMULATOR_BUILD
    QT += simulator
}

load(qt_module)

QMAKE_DOCS = $$PWD/../../doc/config/qtsensors.qdocconf

PUBLIC_HEADERS += \
           qsensorbackend.h\
           qsensormanager.h\
           qsensorplugin.h\
           qsensorsglobal.h

PRIVATE_HEADERS += \
           sensorlog_p.h\

SOURCES += qsensorbackend.cpp\
           qsensormanager.cpp\
           qsensorplugin.cpp\

SOURCES += \
    gestures/qsensorgesture.cpp \
    gestures/qsensorgesturerecognizer.cpp \
    gestures/qsensorgesturemanager.cpp \
    gestures/qsensorgesturemanagerprivate.cpp \
    gestures/qsensorgestureplugininterface.cpp

GESTURE_HEADERS += \
    gestures/qsensorgesture.h\
    gestures/qsensorgesture_p.h\
    gestures/qsensorgesturerecognizer.h \
    gestures/qsensorgesturemanager.h \
    gestures/qsensorgesturemanagerprivate_p.h \
    gestures/qsensorgestureplugininterface.h

simulator {
    SOURCES += gestures/simulatorgesturescommon.cpp
    GESTURE_HEADERS += gestures/simulatorgesturescommon_p.h
}

# 3 files per sensor (including QSensor)
SENSORS=\
    qsensor\
    qaccelerometer\
    qambientlightsensor\
    qcompass\
    qlightsensor\
    qmagnetometer\
    qorientationsensor\
    qproximitysensor\
    qirproximitysensor\
    qrotationsensor\
    qtapsensor\
    qgyroscope\

for(s,SENSORS) {
    # Client API
    PUBLIC_HEADERS += $${s}.h
    SOURCES        += $${s}.cpp
    # Private header
    PRIVATE_HEADERS += $${s}_p.h
}

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS $$GESTURE_HEADERS
