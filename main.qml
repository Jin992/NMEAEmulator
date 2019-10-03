import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.4

Window {
    id:root
    width: 648
    height: 512
    visible: true
    color: "white"
    title: "NMEA Emulator"

    property var markerItem: ( {} )
    property var ggaMsg: ""
    property var rmcMsg: ""

    function emulateNMEAgga() {
        ggaMsg = markerPin.generateGGAmsg()
        serialPort.writeToSerial(ggaMsg)
    }

    function emulateNMEArmc() {
        rmcMsg = markerPin.generateRMCmsg()
        serialPort.writeToSerial(rmcMsg)
    }


    Timer  {
        id: elapsedTimer
        interval: 160;
        running: serialPort.timerTrigger;
        repeat: true;
        onTriggered: root.emulateNMEAgga()
    }

    Timer  {
        id: rmcTimer
        interval: 160;
        running: serialPort.timerTrigger;
        repeat: true;
        onTriggered: root.emulateNMEArmc()
    }

    Button {
        text: qsTr("Config")
        x: 10
        y: 10
        height: 22
        z: 1
        onClicked: {
            var component = Qt.createComponent("ConfigPanel.qml");
            var configWindow = component.createObject(this);
            configWindow.show()
        }
    }

    Button {
        id: emulateNMEAbnt
        width: 90
        height: 22
        text: serialPort.timerTrigger ? "Stop NMEA" : "Emulate NMEA"
        x: parent.width - width - 10
        y: 10
        z:1
        onClicked: {
            serialPort.timerTrigger = !serialPort.timerTrigger
        }
    }

    RoundButton{
        id:nmeaIndicator
        x: parent.width - emulateNMEAbnt.width - width - 20
        y: 10
        height: 24
        width: 24
        z:1
        Image {
            id: name
            y: 5
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent
            source: serialPort.timerTrigger ? "qrc:/res/green_gear.png" : "qrc:/res/red_gear.png"
        }
    }


    Label {
        id: latLabel
        x: parent.width - 175
        y: parent.height - 45
        z:1
        width: 65
        text: qsTr("Latitude:")
        font.pointSize: 12
    }

    Label {
        id: lonLable
        x: parent.width - 175
        y: parent.height - 27
        z:1
        text: qsTr("Longitude:")
        font.pointSize: 12
    }

    Label {
        id: latVal
        x: parent.width - 95
        y: parent.height - 45
        z:1
        text: "50.27"
        font.pointSize: 12
    }

    Label {
        id: lonVal
        x: parent.width - 95
        y: parent.height - 27
        z:1
        text: qsTr("30.71")
        font.pointSize: 12
    }

    Plugin {
        id: mapPlugin
        name: "osm"
    }
    // Map plugin
    Map {
        id: mapview
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(50.45, 30.52) //
        zoomLevel: 14
    }

    Label {
        id: ggaValInfo
        x: 10
        y: parent.height - height - 27
        width: 311
        height: 13
        text: ggaMsg
    }

    Label {
        id: rmcValInfo
        x: parent.x + 10
        y: parent.height - height - 46
        width: 311
        height: 13
        text: rmcMsg
    }

    Component {
        id: mapcomponent
        MapQuickItem {
            id: marker
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: position
            z:1

            sourceItem: Image {
                id: image
                sourceSize.width: 15
                sourceSize.height: 15
                source: "qrc:/res/pin.png"
            }
        }
    }

    MouseArea {
        anchors.topMargin: 0
        anchors.fill: parent
        acceptedButtons:Qt.RightButton
        onClicked:  {
            var coordinate = mapview.toCoordinate(Qt.point(mouse.x,mouse.y))
            markerPin.storeMarker(coordinate)
            mapview.removeMapItem(markerItem)
            latVal.text = markerPin.getLat().toFixed(6)
            lonVal.text = markerPin.getLon().toFixed(6)
            markerItem = Qt.createQmlObject('import QtQuick 2.0; import QtLocation 5.6; MapQuickItem{ sourceItem: Rectangle{ width: 2; height: 2; radius: 1; color:"red"}}', mapview, "dynamic");
            markerItem.anchorPoint = Qt.point(2,2)
            markerItem.coordinate = coordinate;
            markerItem.zoomLevel = 10
            mapview.addMapItem(markerItem);

            //markerModel.addMarker(coordinate)
            //print(coordinate)

        }


    }

    ComboBox {
        id: comboBox
        x: parent.width - emulateNMEAbnt.width - nmeaIndicator.width - width - 30
        y: 10
        width: 154
        height: 22

        model: ListModel {
            id: fixModel
            ListElement { text: "0 = invalid" }
            ListElement { text: "1 = GPS fix (SPS)" }
            ListElement { text: "2 = DGPS fix" }
            ListElement { text: "3 = PPS fix" }
            ListElement { text: "4 = Real Time Kinematic" }
            ListElement { text: "5 = Float RTK" }
            ListElement { text: "6 = estimated (dead reckoning)" }
            ListElement { text: "7 = Manual input mode" }
            ListElement { text: "8 = Simulation mode" }
        }
        onCurrentIndexChanged: {
            print("Clicked")
            fixModel.get(comboBox.currentIndex).text
            if (fixModel.get(comboBox.currentIndex).text === "0 = invalid") {
                markerPin.gpsFix = 0
            } else if (fixModel.get(comboBox.currentIndex).text === "1 = GPS fix (SPS)") {
                markerPin.gpsFix = 1
            } else if (fixModel.get(comboBox.currentIndex).text === "2 = DGPS fix") {
                markerPin.gpsFix = 2
            } else if (fixModel.get(comboBox.currentIndex).text === "3 = PPS fix") {
                markerPin.gpsFix = 3
            } else if (fixModel.get(comboBox.currentIndex).text === "4 = Real Time Kinematic") {
                markerPin.gpsFix = 4
            } else if (fixModel.get(comboBox.currentIndex).text === "5 = Float RTK") {
                markerPin.gpsFix = 5
            } else if (fixModel.get(comboBox.currentIndex).text === "6 = estimated (dead reckoning)") {
                markerPin.gpsFix = 6
            } else if (fixModel.get(comboBox.currentIndex).text === "7 = Manual input mode") {
                markerPin.gpsFix = 7
            } else if (fixModel.get(comboBox.currentIndex).text === "8 = Simulation mode") {
                markerPin.gpsFix = 8
            }
        }
    }

    Button {
        id: rmc
        x: parent.width - emulateNMEAbnt.width - nmeaIndicator.width - comboBox.width - width - 40
        y: 10
        width: 64
        height: 22
        text: markerPin.rmcStatus  ? "Active" : "Void"
        onClicked: markerPin.rmcStatus ? markerPin.rmcStatus = 0 : markerPin.rmcStatus = 1
    }
    Rectangle {
        id: rectangle
        x: parent.width - emulateNMEAbnt.width - nmeaIndicator.width - comboBox.width - rmc.width - width - 50
        y: 10
        width: 140
        height: 22
        color: "#ffffff"
        border.width: 2
        TextInput {
            id:sufixInput
            width: 60
            text: markerPin.gpsSuffix
            anchors.topMargin: 5
            anchors.fill: parent
            font.pointSize: 10
        }
        Button {
            height: 22
            x:60
            width: 80
            text: "Set suffix"
            onClicked: {
                markerPin.gpsSuffix = sufixInput.text
            }
        }
    }






}

/*##^## Designer {
    D{i:60;anchors_width:70;anchors_x:0;anchors_y:0}
}
 ##^##*/
