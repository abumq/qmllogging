import QtQuick 2.0
import org.easylogging.qml 1.1
import org.easylogging.qml.simplechat 1.0

Rectangle {
    width: 360
    height: 360
    MouseArea {
        anchors.fill: parent
        onClicked: {
            txtMessage.forceActiveFocus()
            txtMessage.selectAll()
        }

        Text {
            id: txtMessages
            x: 0
            y: 0
            width: 300
            height: 300
            text: qsTr("")
            font.pixelSize: 12
        }

        TextEdit {
            id: txtMessage
            x: 30
            y: 314
            width: 300
            height: 20
            text: qsTr("Enter your message here...")
            font.pixelSize: 12
            Keys.onReturnPressed: {
                if (txtMessage.text.length > 0) {
                    Messenger.sendMessage(txtMessage.text)
                    txtMessages.text = txtMessages.text + "\n" + txtMessage.text
                    txtMessage.text = ""
                } else {
                    Log.warn("Ignoring empty message")
                }
            }
        }
    }
}
