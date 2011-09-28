/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

//Import the declarative plugins
import QtQuick 2.0
import QtSensors 5.0

/* Layout
                                tiltrect
                               /
------------------------------/
| @ Facing Up  <-------------------- selButtonFacingUp
| O Left Up    <-------------------- selButtonLeftUp
| O Top Down   <-------------------- selButtonTopDown
| O Face Down  <-------------------- selButtonFaceDown
| O Right Up   <-------------------- selButtonRightUp
| O Top Up     <-------------------- selButtonTopUp
| ---------
| |degree |    <-------------------- useRadian
| ---------
| X Rotation: 0  <------------------ xrottext
| Y Rotation: 0  <------------------ yrottext
| ---------
| |start  |    <-------------------- tiltStart
| ---------                     ablrect
------------------------------ /
------------------------------/
| Ambient light: -  <--------------- abltext
| ---------
| |start  |    <-------------------- ablStart
| ---------                     proxirect
------------------------------ /
------------------------------/
| Proximity: false  <--------------- proxitext
| ---------
| |start  |    <-------------------- proxiStart
| ---------
------------------------------

*/

Rectangle {
    width: 320
    height: 480

    Rectangle {
        id: tiltrect
        x: 2
        y: 22
        width: 315
        height: 270
        border.width: 1

        TiltSensor {
            id: tilt
            radian: false
            measureFrom: TiltSensor.FaceUp
            running: false
        }

        SelectionButton{
            id: selButtonFacingUp
            x: 5
            y: 20
            checked: true
            text: "Facing Up"
            onClicked:{
                selButtonLeftUp.setCheck(false);
                selButtonTopDown.setCheck(false);
                selButtonFaceDown.setCheck(false);
                selButtonRightUp.setCheck(false);
                selButtonTopUp.setCheck(false);
                tilt.measureFrom=TiltSensor.FaceUp;
            }
        }
        SelectionButton{
            id: selButtonLeftUp
            x: 5
            y: 45
            checked: false
            text: "Left Up"
            onClicked:{
                selButtonFacingUp.setCheck(false);
                selButtonTopDown.setCheck(false);
                selButtonFaceDown.setCheck(false);
                selButtonRightUp.setCheck(false);
                selButtonTopUp.setCheck(false);
                tilt.measureFrom=TiltSensor.LeftUp;
            }
        }
        SelectionButton{
            id: selButtonTopDown
            x: 5
            y: 70
            checked: false
            text: "Top Down"
            onClicked:{
                selButtonFacingUp.setCheck(false);
                selButtonLeftUp.setCheck(false);
                selButtonFaceDown.setCheck(false);
                selButtonRightUp.setCheck(false);
                selButtonTopUp.setCheck(false);
                tilt.measureFrom=TiltSensor.TopDown;
            }
        }
        SelectionButton{
            id: selButtonFaceDown
            x: 5
            y: 95
            checked: false
            text: "Face Down"
            onClicked:{
                selButtonFacingUp.setCheck(false);
                selButtonLeftUp.setCheck(false);
                selButtonTopDown.setCheck(false);
                selButtonRightUp.setCheck(false);
                selButtonTopUp.setCheck(false);
                tilt.measureFrom=TiltSensor.FaceDown;
            }
        }
        SelectionButton{
            id: selButtonRightUp
            x: 5
            y: 120
            checked: false
            text: "Right Up"
            onClicked:{
                selButtonFacingUp.setCheck(false);
                selButtonLeftUp.setCheck(false);
                selButtonTopDown.setCheck(false);
                selButtonFaceDown.setCheck(false);
                selButtonTopUp.setCheck(false);
                tilt.measureFrom=TiltSensor.RightUp;
            }
        }
        SelectionButton{
            id: selButtonTopUp
            x: 5
            y: 145
            checked: false
            text: "Top Up"
            onClicked:{
                selButtonFacingUp.setCheck(false);
                selButtonLeftUp.setCheck(false);
                selButtonTopDown.setCheck(false);
                selButtonFaceDown.setCheck(false);
                selButtonRightUp.setCheck(false);
                tilt.measureFrom=TiltSensor.TopUp;
            }
        }

        Button{
            id: useRadian
            x: 5
            y: 170
            text: "degree"
            checkColor: "lightblue"
            unCheckColor: "lightyellow"
            checked: tilt.radian
            color: "lightyellow"

            onClicked:{
                tilt.radian = useRadian.checked;
                if (useRadian.checked)
                    useRadian.text = "radian";
                else
                    useRadian.text = "degree";
            }
        }

        Text {
            id: xrottext
            x:5
            y:195
            text: "X Rotation: " + tilt.xRotation
        }
        Text {
            id: yrottext
            x:5
            y:220
            text: "Y Rotation: " + tilt.yRotation
        }
        Button{
            id: tiltStart
            x: 5
            y: 245
            text: "start"
            checkColor: "lightblue"
            unCheckColor: "lightyellow"
            checked: tilt.running
            color: "lightyellow"

            onClicked:{
                tilt.running = tiltStart.checked;
                if (tiltStart.checked)
                    tiltStart.text = "running";
                else
                    tiltStart.text = "stopped";
            }
        }
    }

    Rectangle {
        id: ambientlightrect
        x: 2
        y: tiltrect.y + tiltrect.height + 10
        width: 315
        height: 70
        border.width: 1

        AmbientLightSensor {
            id: ambientlight
            enabled: false
            onLightLevelChanged:{
                if (ambientlight.lightLevel == AmbientLightSensor.Unknown)
                    ambientlighttext.text = "Ambient light: Unknown";
                else if (ambientlight.lightLevel == AmbientLightSensor.Dark)
                    ambientlighttext.text = "Ambient light: Dark";
                else if (ambientlight.lightLevel == AmbientLightSensor.Twilight)
                    ambientlighttext.text = "Ambient light: Twilight";
                else if (ambientlight.lightLevel == AmbientLightSensor.Light)
                    ambientlighttext.text = "Ambient light: Light";
                else if (ambientlight.lightLevel == AmbientLightSensor.Bright)
                    ambientlighttext.text = "Ambient light: Bright";
                else if (ambientlight.lightLevel == AmbientLightSensor.Sunny)
                    ambientlighttext.text = "Ambient light: Sunny";
            }
        }
        Text {
            id: ambientlighttext
            x: 5
            y: 10
            text: "Ambient light: -"
        }
        Button{
            id: ambientlightStart
            x: 5
            y: 35
            text: "start"
            checkColor: "lightblue"
            unCheckColor: "lightyellow"
            checked: ambientlight.enabled
            color: "lightyellow"

            onClicked:{
                ambientlight.enabled = ambientlightStart.checked;
                if (ambientlightStart.checked)
                    ambientlightStart.text = "running";
                else
                    ambientlightStart.text = "stopped";
            }
        }
    }

    Rectangle {
        id: proxirect
        x: 2
        y: ambientlightrect.y + ambientlightrect.height + 10
        width: 315
        height: 70
        border.width: 1

        ProximitySensor {
            id: proxi
            enabled: true
        }
        Text {
            id: proxitext
            x: 5
            y: 10
            text: "Proximity: " + proxi.near
        }
        Button{
            id: proxiStart
            x: 5
            y: 35
            text: "start"
            checkColor: "lightblue"
            unCheckColor: "lightyellow"
            checked: proxi.enabled
            color: "lightyellow"

            onClicked:{
                proxi.enabled = proxiStart.checked;
                if (proxiStart.checked)
                    proxiStart.text = "running";
                else
                    proxiStart.text = "stopped";
            }
        }
    }
}
