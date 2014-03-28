import QtQuick 2.0
import org.easylogging.qml 1.0

Rectangle {
    width: 360
    height: 360
    border.color: "#ff0000"
    border.width: 3
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
            Log.timeBegin("onClicked")
            Log.count("onClicked times")
            if (rect == null) {
                rect = Qt.createComponent("MyCircle.qml").createObject(parent, {x: mouseX, y: mouseY})
                Log.info("1. This is " + rect)
                Log.info("2. This is ", rect)
                Log.verbose(2, "This is verbose")
            }
            Log.timeEnd("onClicked")
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
                    return;
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
