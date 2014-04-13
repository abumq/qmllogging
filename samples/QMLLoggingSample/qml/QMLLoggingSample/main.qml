import QtQuick 2.0
import org.easylogging.qml 1.2

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
            log.timeBegin("onClicked")
            log.count("onClicked times")
            if (rect == null) {
                rect = Qt.createComponent("MyCircle.qml").createObject(parent, {x: mouseX, y: mouseY})
                log.info("1. This is ", rect, " and this is continuation of log")
                log.info("2. This is ", rect)
                log.verbose(2, "This is verbose")
            }
            log.assert(rect != null, "Rect is not suppose to be null now")
            log.timeEnd("onClicked")
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
                    log.error("Please click area first")
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
