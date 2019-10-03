import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Window 2.0

Window {
    id:config
    height:134
    width: 430

    title: "Serial Port Config"

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    property var comModel: ({})
    property bool comStatus: false

    Component.onCompleted: {
        comModel = serialPort.scanSerial()

    }


    Rectangle {
        id: configBody
        height:120
        width: 430
        color: "#ffffff"

        ComboBox {
            id: serialPorttListBox
            x: 8
            y: 30
            width: 196
            height: 26
            model:comModel
        }

        Label {
            id: serialPortListLabel
            x: 8
            y: 8
            width: 196
            height: 16
            text: qsTr("Available serial ports:")
            font.pointSize: 11
        }

        Button {
            id: serialConfigApply
            x: 323
            y: 31
            height: 26
            text: comStatus ? "Disconnect" : "Connect"
            onClicked: {
                if (comStatus === false) {
                    serialPort.openSerial(serialPorttListBox.currentText,
                                          serialBaudBox.currentText,
                                          serialDataBitBox.currentText,
                                          serialParityBox.currentText,
                                          serialStopBitBox.currentText)
                    comStatus = true
                } else {
                    serialPort.close()
                    comStatus = false
                }

            }

        }

        Label {
            id: serialBaudLabel
            x: 8
            y: 60
            width: 144
            height: 16
            text: qsTr("Serial Port Baud Rate:")
            font.pointSize: 11
        }

        ComboBox {
            id: serialBaudBox
            x: 8
            y: 82
            width: 146
            height: 26
            model: ListModel {
                id: baudRateModel
                ListElement { text: "9600" }
                ListElement { text: "19200" }
                ListElement { text: "38400" }
                ListElement { text: "57600" }
                ListElement { text: "115200" }
            }
        }

        Label {
            id: serialParityLabel
            x: 257
            y: 60
            height: 16
            text: qsTr("Parity:")
            font.pointSize: 11
        }

        ComboBox {
            id: serialParityBox
            x: 257
            y: 82
            width: 89
            height: 26
            model: ListModel {
                id: parityModel
                ListElement { text: "NoParity" }
                ListElement { text: "EvenParity" }
                ListElement { text: "OddParity" }
                ListElement { text: "SpaceParity" }
                ListElement { text: "MarkParity" }
            }
        }

        ComboBox {
            id: serialDataBitBox
            x: 160
            y: 82
            width: 92
            height: 26
            model: ListModel {
                id: databitModel
                ListElement { text: "5" }
                ListElement { text: "6" }
                ListElement { text: "7" }
                ListElement { text: "8" }
            }
        }

        Label {
            id: serialDataBitLabel
            x: 160
            y: 60
            width: 66
            height: 16
            text: qsTr("Data bits:")
            font.pointSize: 11
        }

        ComboBox {
            id: serialStopBitBox
            x: 352
            y: 82
            width: 71
            height: 26
            model: ListModel {
                id: stopbitModel
                ListElement { text: "1" }
                ListElement { text: "1.5" }
                ListElement { text: "2" }
            }
        }

        Label {
            id: serialStopBitLabel
            x: 352
            y: 63
            height: 16
            text: qsTr("Stop Bit:")
            font.pointSize: 11
        }

        Label {
            id: serialConMsg
            x: 29
            y: 114
            width: 373
            height: 13
            text: serialPort.serialConnMsg
            font.pointSize: 9
            horizontalAlignment: Text.AlignHCenter
        }
    }

}

