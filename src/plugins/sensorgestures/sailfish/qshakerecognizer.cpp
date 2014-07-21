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

#include "qshakerecognizer.h"
#include <math.h>


QT_BEGIN_NAMESPACE

QShakeSensorGestureRecognizer::QShakeSensorGestureRecognizer(QObject *parent)
    : QSensorGestureRecognizer(parent)
    , active(0)
    , shaking(0)
    , shakeCount(0)
{
    timerTimeout = 450;
    qDebug();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    timer->setSingleShot(true);
    timer->setInterval(timerTimeout);

}

QShakeSensorGestureRecognizer::~QShakeSensorGestureRecognizer()
{
}

void QShakeSensorGestureRecognizer::create()
{
}

bool QShakeSensorGestureRecognizer::start()
{
    qDebug();
    if (QtSensorGestureSensorHandler::instance()->startSensor(QtSensorGestureSensorHandler::Accel)) {
        active = true;
        connect(QtSensorGestureSensorHandler::instance(),SIGNAL(accelReadingChanged(QAccelerometerReading *)),
                this,SLOT(accelChanged(QAccelerometerReading *)));
    } else {
        active = false;
        qDebug() << "Could not start shake gesture";
    }
    prevData.x = 0;
    prevData.y = 0;
    prevData.z = 0;
    shakeCount = 0;
    shaking = false;

    return active;
}

bool QShakeSensorGestureRecognizer::stop()
{
    QtSensorGestureSensorHandler::instance()->stopSensor(QtSensorGestureSensorHandler::Accel);
    disconnect(QtSensorGestureSensorHandler::instance(),SIGNAL(accelReadingChanged(QAccelerometerReading *)),
            this,SLOT(accelChanged(QAccelerometerReading *)));
    active = false;
    return active;
}

bool QShakeSensorGestureRecognizer::isActive()
{
    return active;
}

QString QShakeSensorGestureRecognizer::id() const
{
    return QString("Sailfish.shake");
}

#define NUMBER_SHAKES 3
#define THRESHOLD 11

void QShakeSensorGestureRecognizer::accelChanged(QAccelerometerReading *reading)
{
    qreal x = reading->x();
    qreal y = reading->y();
    qreal z = reading->z();

    currentData.x = x;
    currentData.y = y;
    currentData.z = z;

    if (qAbs(prevData.x - currentData.x)  < 1
            && qAbs(prevData.y - currentData.y)  < 1
            && qAbs(prevData.z - currentData.z)  < 1) {
        prevData.x = currentData.x;
        prevData.y = currentData.y;
        prevData.z = currentData.z;
        return;
    }

    bool wasShake = checkForShake(prevData, currentData, THRESHOLD);
    if (!shaking && wasShake &&
        shakeCount >= NUMBER_SHAKES) {
        shaking = true;
        shakeCount = 0;
        Q_EMIT shake();
        Q_EMIT detected("shake");

    } else if (wasShake) {

        shakeCount++;
        if (shakeCount > NUMBER_SHAKES) {
            timer->start();
        }
    }

    prevData.x = currentData.x;
    prevData.y = currentData.y;
    prevData.z = currentData.z;
}

void QShakeSensorGestureRecognizer::timeout()
{
    shakeCount = 0;
    shaking = false;
}

bool QShakeSensorGestureRecognizer::checkForShake(ShakeData prevSensorData, ShakeData currentSensorData, qreal threshold)
{
    const double deltaX = qAbs(prevSensorData.x - currentSensorData.x);
    const double deltaY = qAbs(prevSensorData.y - currentSensorData.y);
    const double deltaZ = qAbs(prevSensorData.z - currentSensorData.z);

    if (deltaX > 6 || deltaY > 6 || deltaZ> 6)
        qDebug() << deltaX << deltaY << deltaZ;

    return (deltaX > threshold
            || deltaY > threshold
            || deltaZ > threshold);
}



QT_END_NAMESPACE
