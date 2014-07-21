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


#include "qtwistsensorgesturerecognizer.h"

#include <QtCore/qmath.h>

QT_BEGIN_NAMESPACE

#define RADIANS_TO_DEGREES 57.2957795
#define TIMER_TIMEOUT 750
QTwistSensorGestureRecognizer::QTwistSensorGestureRecognizer(QObject *parent)
    : QSensorGestureRecognizer(parent)
    , active(0)
{
    qDebug();
}

QTwistSensorGestureRecognizer::~QTwistSensorGestureRecognizer()
{
}

void QTwistSensorGestureRecognizer::create()
{
}

QString QTwistSensorGestureRecognizer::id() const
{
    return QString("Sailfish.twist");
}

bool QTwistSensorGestureRecognizer::start()
{
    qDebug();
    if (QtSensorGestureSensorHandler::instance()->startSensor(QtSensorGestureSensorHandler::Gyroscope)) {
            connect(QtSensorGestureSensorHandler::instance(),SIGNAL(gyroReadingChanged(QGyroscopeReading *)),
                    this,SLOT(gyroReadingChanged(QGyroscopeReading *)));
    } else {
        qDebug() << "Could not start twist gesture: Gyro";
        active = false;
    }

    return active;
}

bool QTwistSensorGestureRecognizer::stop()
{
    QtSensorGestureSensorHandler::instance()->stopSensor(QtSensorGestureSensorHandler::Gyroscope);
    disconnect(QtSensorGestureSensorHandler::instance(),SIGNAL(gyroReadingChanged(QGyroscopeReading *)),
            this,SLOT(gyroReadingChanged(QGyroscopeReading *)));
    active = false;
    return active;
}

bool QTwistSensorGestureRecognizer::isActive()
{
    return active;
}

void QTwistSensorGestureRecognizer::gyroReadingChanged(QGyroscopeReading *reading)
{
    if (qAbs(reading->y()) < 5)
        return;

    qreal av;
    if (gyroYs.count() > 3) {
        gyroYs.removeFirst();
        av = (reading->y() + gyroYs.at(0) + gyroYs.at(1) + gyroYs.at(2)) / 4;
        gyroYs.append(av);
//        if (qAbs(av) > 100)
//            qDebug() << "average" << av << reading->y() << lastY;

//        if (av > 150 && reading->y() < -100)
//                qDebug() << "<<<<<<<<<<<<<<<<<<< turn >>>>>>>>>>>>>>>>>>>>>";
    } else {
        gyroYs.append(reading->y());
    }
    if (av > 150 && reading->y() < -75) {
        Q_EMIT twistRight();
        Q_EMIT detected("twistRight");
    }
    if (av < -100 && reading->y() > 75 && lastY) {
        Q_EMIT twistLeft();
        Q_EMIT detected("twistLeft");
    }
    lastY = reading->y();
}

QT_END_NAMESPACE
