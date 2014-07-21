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

#include "qpickupsensorgesturerecognizer.h"

#include <QtCore/qmath.h>

#define RADIANS_TO_DEGREES 57.2957795
#define TIMER_TIMEOUT 250

QT_BEGIN_NAMESPACE

QPickupSensorGestureRecognizer::QPickupSensorGestureRecognizer(QObject *parent)
    : QSensorGestureRecognizer(parent)
    , accelReading(0)
    , active(0)
    , pXaxis(0)
    , pYaxis(0)
    , pZaxis(0)
    , lastpitch(0)
    , detecting(0)
{
}

QPickupSensorGestureRecognizer::~QPickupSensorGestureRecognizer()
{
}

void QPickupSensorGestureRecognizer::create()
{
}

QString QPickupSensorGestureRecognizer::id() const
{
    return QString("Sailfish.pickup");
}

bool QPickupSensorGestureRecognizer::start()
{
    qDebug();

    if (QtSensorGestureSensorHandler::instance()->startSensor(QtSensorGestureSensorHandler::Rotation)) {
         if (QtSensorGestureSensorHandler::instance()->startSensor(QtSensorGestureSensorHandler::Gyroscope)) {
            active = true;
            connect(QtSensorGestureSensorHandler::instance(),SIGNAL(rotationReadingChanged(QRotationReading *)),
                    this,SLOT(rotationChanged(QRotationReading *)));
            connect(QtSensorGestureSensorHandler::instance(),SIGNAL(gyroReadingChanged(QGyroscopeReading *)),
                    this,SLOT(gyroReadingChanged(QGyroscopeReading *)));
        } else {
            QtSensorGestureSensorHandler::instance()->stopSensor(QtSensorGestureSensorHandler::Rotation);
            active = false;
        }
    }
    clear();

    return active;

}

bool QPickupSensorGestureRecognizer::stop()
{
    QtSensorGestureSensorHandler::instance()->stopSensor(QtSensorGestureSensorHandler::Rotation);
    QtSensorGestureSensorHandler::instance()->stopSensor(QtSensorGestureSensorHandler::Gyroscope);

    disconnect(QtSensorGestureSensorHandler::instance(),SIGNAL(accelReadingChanged(QAccelerometerReading*)),
            this,SLOT(accelChanged(QAccelerometerReading *)));
    disconnect(QtSensorGestureSensorHandler::instance(),SIGNAL(gyroReadingChanged(QGyroscopeReading *)),
            this,SLOT(gyroReadingChanged(QGyroscopeReading *)));
    active = false;

    return active;
}

bool QPickupSensorGestureRecognizer::isActive()
{
    return active;
}

#define PICKUP_BOTTOM_THRESHOLD 25
#define PICKUP_TOP_THRESHOLD 80
#define PICKUP_ANGLE_THRESHOLD 25
#define PICKUP_ROLL_THRESHOLD 13

void QPickupSensorGestureRecognizer::rotationChanged(QRotationReading *reading)
{
    accelReading = reading;
    const qreal x = reading->x();
    const qreal y = reading->y();
    const qreal z = reading->z();
    const qreal xdiff =  pXaxis - x;
    const qreal ydiff = pYaxis - y;
    const qreal zdiff =  pZaxis - z;

    qreal pitch = reading->x();//qAtan(y / qSqrt(x*x + z*z)) * RADIANS_TO_DEGREES;
    qreal roll = reading->y();//qAtan(x / qSqrt(y*y + z*z)) * RADIANS_TO_DEGREES;

    if ((qAbs(xdiff) < 0.7 && qAbs(ydiff) < .7 && qAbs(zdiff) < .7)
            || z < 0) {
        detecting = false;
    } else if (pitch > PICKUP_BOTTOM_THRESHOLD && pitch < PICKUP_TOP_THRESHOLD) {
        detecting = true;
     }

    if ( pitchList.count() > 21) {
        pitchList.removeFirst();
    }
    if ( rollList.count() > 21) {
        rollList.removeFirst();
    }

    if (pitch > 1) {
        pitchList.append(pitch);
    }
    if (roll > 1) {
        rollList.append(roll);
    }

    if (detecting && pitchList.count() > 5 ) {
       timeout();
    }

    lastpitch = pitch;
    pXaxis = x;
    pYaxis = y;
    pZaxis = z;
}

void QPickupSensorGestureRecognizer::timeout()
{
    qreal averageRoll = 0;
    for (int r = 0; r < rollList.count(); r++) {
        averageRoll += rollList.at(r);
    }
    averageRoll /= rollList.count();

    if (averageRoll >  PICKUP_ROLL_THRESHOLD) {
        clear();
        return;
    }
    if (pitchList.isEmpty()
            || pitchList.at(0) > PICKUP_BOTTOM_THRESHOLD) {
        clear();
        return;
    }

    qreal previousPitch = 0;
    qreal startPitch = -1.0;
    int goodCount = 0;

    qreal averagePitch = 0;
    for (int i = 0; i < pitchList.count(); i++) {
        averagePitch += pitchList.at(i);
        if (previousPitch < pitchList.at(i)
                && qAbs(pitchList.at(i)) - qAbs(previousPitch) < 20) {
            if (goodCount == 1 && previousPitch != 0) {
                startPitch = previousPitch;
            }
            goodCount++;
        }

        previousPitch = pitchList.at(i);
    }
    averagePitch /= pitchList.count();

    if (averagePitch < 5) {
        clear();
        return;
    }

    if (goodCount >= 3 &&
            (pitchList.last() < PICKUP_TOP_THRESHOLD
             && pitchList.last() > PICKUP_BOTTOM_THRESHOLD)
            && startPitch > 0
            && (pitchList.last() - startPitch) > PICKUP_ANGLE_THRESHOLD) {
        Q_EMIT pickup();
        Q_EMIT detected("pickup");
    }
    clear();
}

void QPickupSensorGestureRecognizer::clear()
{
    pitchList.clear();
    detecting = false;
}

void QPickupSensorGestureRecognizer::gyroReadingChanged(QGyroscopeReading *reading)
{
    if (qAbs(reading->x()) < 50)
        return;
    qDebug() << reading->x() /*<< reading->y() << reading->z()*/;

    if (gyroList.count() > 6) {
        gyroList.removeFirst();
    }
    gyroList.append(reading->x());

    qreal average = 0;
    Q_FOREACH (qreal i, gyroList) {
        average += i;
    }
    average /= gyroList.count();

    qDebug() << "    "<< average;
}

QT_END_NAMESPACE

