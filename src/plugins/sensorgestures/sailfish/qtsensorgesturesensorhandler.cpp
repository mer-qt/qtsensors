/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QDebug>

#include "qtsensorgesturesensorhandler.h"

QtSensorGestureSensorHandler::QtSensorGestureSensorHandler(QObject *parent) :
    QObject(parent),
    accel(0),
    orientation(0),
    proximity(0),
    irProx(0),
    tapSensor(0),
    gyro(0),
    rotation(0)
{
    qDebug();
}

QtSensorGestureSensorHandler* QtSensorGestureSensorHandler::instance()
{
    static QtSensorGestureSensorHandler *instance = 0;
    if (!instance) {
        instance = new QtSensorGestureSensorHandler;
    }
    return instance;
}

void QtSensorGestureSensorHandler::accelChanged()
{
    Q_EMIT accelReadingChanged(accel->reading());
}

void QtSensorGestureSensorHandler::orientationChanged()
{
    Q_EMIT orientationReadingChanged(orientation->reading());
}

void QtSensorGestureSensorHandler::proximityChanged()
{
    Q_EMIT proximityReadingChanged(proximity->reading());
}

void QtSensorGestureSensorHandler::irProximityChanged()
{
    Q_EMIT irProximityReadingChanged(irProx->reading());
}

void QtSensorGestureSensorHandler::doubletap()
{
    Q_EMIT dTabReadingChanged(tapSensor->reading());
}

void QtSensorGestureSensorHandler::gyroChanged()
{
    Q_EMIT gyroReadingChanged(gyro->reading());
}

void QtSensorGestureSensorHandler::rotationChanged()
{
    Q_EMIT rotationReadingChanged(rotation->reading());
}

bool QtSensorGestureSensorHandler::startSensor(SensorGestureSensors sensor)
{
    bool ok = true;
    qDebug() << sensor;

    switch (sensor) {
    case Accel:
        //accel
        if (accel == 0x0) {
            qDebug() << "Accelerometer";

            accel = new QAccelerometer(this);
            ok = accel->connectToBackend();
            accel->setProperty("alwaysOn",true);
            if (!ok) {
                qDebug() << "Could not connect to backend";
                return false;
            }

            accel->setDataRate(100);
            qoutputrangelist outputranges = accel->outputRanges();

            if (outputranges.count() > 0)
                accelRange = (int)(outputranges.at(0).maximum);//39
            if (accelRange == 0)
                accelRange = 39; //this should never happen
            qDebug() << outputranges.count() << "accelRange"<< accelRange;

            connect(accel,SIGNAL(readingChanged()),this,SLOT(accelChanged()));
        }
        if (ok && !accel->isActive())
            if (!accel->start())
                qDebug() << "Accelerometer could not start";
        break;
    case Orientation:
        //orientation
        if (orientation == 0x0) {
            qDebug() << "Orientation";

            orientation = new QOrientationSensor(this);
            ok = orientation->connectToBackend( );
            if (!ok) {
                qDebug() << "Could not connect to backend";
                return false;
            }
            orientation->setDataRate(50);
            connect(orientation,SIGNAL(readingChanged()),this,SLOT(orientationChanged()));
        }
        if (ok && !orientation->isActive())
            if (!orientation->start())
        qDebug() << "orientation sensor could not start";

        break;
    case Proximity:
        //proximity
        if (proximity == 0x0) {
            qDebug() << "Proxy";

            proximity = new QProximitySensor(this);
            ok = proximity->connectToBackend();
            if (!ok) {
                qDebug() << "Could not connect to backend";
                return false;
            }
            connect(proximity,SIGNAL(readingChanged()),this,SLOT(proximityChanged()));
        }
        if (ok && !proximity->isActive())
            if (!proximity->start())
                qDebug() << "Proximity could not start";
        break;
    case IrProximity:
        //irproximity
        if (irProx == 0x0) {
            qDebug() << "IrProxy";

            irProx = new QIRProximitySensor(this);
            irProx->setDataRate(50);
            ok = irProx->connectToBackend();
            if (!ok) {
                qDebug() << "Could not connect to backend";
                return false;
            }
            connect(irProx,SIGNAL(readingChanged()),this,SLOT(irProximityChanged()));
        }
        if (ok && !irProx->isActive())
            if (!irProx->start())
                qDebug() << "Ir proxy could not start";

        break;
    case Tap:
        //dtap
        if (tapSensor == 0x0) {
            qDebug() << "Tap";
            tapSensor = new QTapSensor(this);
            ok = tapSensor->connectToBackend();
            if (!ok) {
                qDebug() << "Could not connect to backend";
                return false;
            }
            connect(tapSensor,SIGNAL(readingChanged()),this,SLOT(doubletap()));
        }
        if (ok && !tapSensor->isActive())
            tapSensor->start();
        break;
    case Gyroscope:
        //Gyroscope
        if (gyro == 0x0) {
            qDebug() << "Gyro";
            gyro = new QGyroscope(this);
            ok = gyro->connectToBackend();
            if (!ok) {
                qDebug() << "Could not connect to backend";
                return false;
            }
            connect(gyro,SIGNAL(readingChanged()),this,SLOT(gyroChanged()));
        }
        if (ok && !gyro->isActive())
            gyro->setDataRate(50);
            if (!gyro->start())
                qDebug() << "Gyro could not start";
        break;
    case Rotation:
        //Rotation
        if (rotation == 0x0) {
            qDebug() << "Rotation";
            rotation = new QRotationSensor(this);
            ok = rotation->connectToBackend();
            if (!ok) {
                qDebug() << "Could not connect to backend";
                return false;
            }
            connect(rotation,SIGNAL(readingChanged()),this,SLOT(rotationChanged()));
        }
        if (ok && !rotation->isActive())
            if (!rotation->start())
                qDebug() << "rotation could not start";
        break;
    };
    int val = usedSensorsMap.value(sensor);
    usedSensorsMap.insert(sensor,++val);

    return ok;
}

void QtSensorGestureSensorHandler::stopSensor(SensorGestureSensors sensor)
{
      qDebug() << __FUNCTION__ << sensor;
    if (usedSensorsMap.value(sensor) == 0)
        return;
    int val = usedSensorsMap.value(sensor);
    usedSensorsMap.insert(sensor,--val);
    switch (sensor) {
    case Accel:
        //accel
        if (usedSensorsMap.value(sensor) == 0) {
            accel->stop();
        }
        break;
    case Orientation:
        if (usedSensorsMap.value(sensor) == 0) {
            orientation->stop();
        }
        //orientation
        break;
    case Proximity:
        if (usedSensorsMap.value(sensor) == 0) {
            proximity->stop();
        }
        //proximity
        break;
    case IrProximity:
        if (usedSensorsMap.value(sensor) == 0) {
            irProx->stop();
        }
        //irproximity
        break;
    case Tap:
        if (usedSensorsMap.value(sensor) == 0) {
            tapSensor->stop();
        }
        //dtap
        break;
    case Gyroscope:
        if (usedSensorsMap.value(sensor) == 0) {
            gyro->stop();
        }
        //gyro
        break;
    case Rotation:
        if (usedSensorsMap.value(sensor) == 0) {
            rotation->stop();
        }
        //gyro
        break;
        //Rotation
    default:
        break;
    };
}
