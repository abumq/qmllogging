import QtQuick 2.0
import org.easylogging.qml 1.0

Rectangle {
    width: 360
    height: 360
    property Rectangle rect: null
    Text {
        id: mainText
        text: qsTr("Easylogging++ QMLLogging Sample")
        font.pixelSize: 15
        anchors.centerIn: parent
        state: "YELLOW"
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (rect == null) {
                rect = Qt.createComponent("MyCircle.qml").createObject(parent, {x: mouseX, y: mouseY})
                Log.info("This is info log")
            }
        }
    }
    
    Rectangle {
        id: clickMeButton
        x: 50
        y: 20
        width: 130
        height: 30
        color: "lightblue"
        border.color: "#000000"
        Text {
            id: myButton
            text: qsTr("Click me")
            x: 35
            y: 8
        }
        MouseArea {
            id: clickMeButtonMouseArea
            anchors.fill: parent
            function toggleState() {
                if (rect == null) {
                    Log.error("Please click area first")
                }
                if (rect.state == "YELLOW")
                    rect.state = "RED"
                else
                    rect.state = "YELLOW"
                if (mainText.state == "YELLOW")
                    mainText.state = "RED"
                else
                    mainText.state = "YELLOW"
            }

            onClicked: {
                toggleState()
                myButton.text = "Click me again and again!";
                clickMeButton.width= 225
            }
        }
    }
    
    Timer {
        interval: 1000; running: true; repeat: false
        onTriggered: {
            clickMeButtonMouseArea.toggleState()
        }
    }
}
