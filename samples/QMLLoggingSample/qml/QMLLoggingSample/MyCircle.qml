import QtQuick 2.0
import org.easylogging.qml 1.3

Rectangle {
    id: rect
    color: "yellow"
    border.color: "black"
    radius: 50
    
    width: 150
    height: 150
    state: "YELLOW"
    Text {
        id: moveMeText
        text: qsTr("Move me")
        x: 25
        y: 25
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons:  Qt.LeftButton
        onDoubleClicked: {
            rect.destroy();
        }
        onPositionChanged: {
            if (mouse.buttons & Qt.LeftButton) {
                rect.x -= (x - mouse.x)
                rect.y -= (y - mouse.y)
                moveMeText.x = rect.x / 2
                moveMeText.y = rect.y / 3
                log.info("Moving circle: ", rect.x, " x ", rect.y)
            }
        }
    }
    states: [
        State {
            name: "YELLOW"
            PropertyChanges { target: rect; color: "yellow" }
            PropertyChanges { target: mainText; color: "red" }
            StateChangeScript { script: log.info("State changed to YELLOW") }
        },
        State {
            name: "RED"
            PropertyChanges { target: rect; color: "red" }
            PropertyChanges { target: mainText; color: "yellow" }
            StateChangeScript {
                script: {
                    log.timeBegin("stateChangedRed")
                    log.info("State changed to RED")
                    log.timeEnd("stateChangedRed")
                }
            }
        }
    ]
    transitions: [
        Transition {
             from: "YELLOW"; to: "RED"
             PropertyAnimation { 
                target: rect
                properties: "color"; 
                duration: 1000 
             }
             PropertyAnimation { 
                target: mainText
                properties: "color"; 
                duration: 1000 
             }
        },
        Transition {
             from: "RED"; to: "YELLOW"
             PropertyAnimation { 
                target: rect
                properties: "color"; 
                duration: 1000 
             }
             PropertyAnimation { 
                target: mainText
                properties: "color"; 
                duration: 1000 
             }
        }

    ]
}
