import QtQuick 2.0
import org.easylogging.qml 1.1
import org.easylogging.qml.simplechat 1.0

Rectangle {
    width: 360
    height: 360
    id: mainRect
    
    signal messageReceived(string msg)
    
    onMessageReceived: {
        txtMessages.text = txtMessages.text + "\n" + msg.trim()
        Log.info("Message received " + msg)
    }

    MouseArea {
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        onClicked: {
            txtMessage.forceActiveFocus()
            txtMessage.selectAll()
        }

        Text {
            id: txtMessages
            x: 10
            y: 10
            width: 342
            height: 239
            text: qsTr("")
            font.pixelSize: 12
        }
        
        TextEdit {
            id: txtMessage
            x: 10
            y: 264
            width: 342
            height: 20
            text: qsTr("Enter your message here...")
            font.pixelSize: 12
            Keys.onReturnPressed: {
                if (txtMessage.text.length > 0) {
                    var message = txtNick.text + txtMessage.text
                    Messenger.sendMessage(txtClientLocation.text.split(":")[0], 
                        txtClientLocation.text.split(":")[1], 
                        message)
                    txtMessages.text = txtMessages.text + "\n" + message
                    txtMessage.text = ""
                } else {
                    Log.warn("Ignoring empty message")
                }
            }

            Rectangle {
                id: rectangle1
                x: 0
                y: 0
                width: 300
                height: 20
                color: "#999999"
                z: -1
            }
        }
        
        Connections {
            target: Server
            onReady: {
                messageReceived(msg)
            }
        }

        TextInput {
            id: txtServerId
            x: 10
            y: 337
            width: 293
            height: 20
            text: "localhost:" + Server.port()
            font.pixelSize: 12
        }

        TextInput {
            id: txtClientLocation
            x: 64
            y: 292
            width: 251
            height: 18
            text: "localhost:15910"
            font.pixelSize: 12
        }

        Rectangle {
            id: btnStart
            x: 300
            y: 337
            width: 50
            height: 20
            color: "#000000"
            z: -1
            MouseArea {
                anchors.bottomMargin: 14
                anchors.fill: parent
                onClicked: {
                    serverObj.start(txtServerId.text.split(":")[1])
                }
            }
            Text {
                id: text1
                x: 5
                y: 0
                anchors.fill: parent
                color: "#ffffff"
                text: qsTr("Start")
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }
        }

        TextInput {
            id: txtNick
            x: 64
            y: 314
            width: 251
            height: 17
            text: qsTr("mkhan3189")
            font.pixelSize: 12
        }

        Text {
            id: textNick
            x: 10
            y: 316
            text: qsTr("Nick:")
            font.pixelSize: 12
        }

        Text {
            id: text2
            x: 8
            y: 293
            text: qsTr("Receiver:")
            font.pixelSize: 12
        }
    }
}
