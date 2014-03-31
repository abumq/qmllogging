import QtQuick 2.0
import org.easylogging.qml 1.0

Rectangle {
    id: rect
    color: "yellow"
    border.color: "black"
    radius: 50
    
    width: 50
    height: 50
    state: "YELLOW"
    MouseArea {
        anchors.fill: parent
        acceptedButtons:  Qt.LeftButton | Qt.RightButton
        onDoubleClicked: {
            rect.destroy();
        }
        onPositionChanged: {
            if (mouse.buttons & Qt.LeftButton) {
                rect.x -= (x - mouse.x)
                rect.y -= (y - mouse.y)
                Log.info("Moving circle: ", rect.x, " x ", rect.y)
            }
        }
    }
    states: [
        State {
            name: "YELLOW"
            PropertyChanges { target: rect; color: "yellow" }
            PropertyChanges { target: mainText; color: "red" }
            StateChangeScript { script: Log.info("State changed to YELLOW") }
        },
        State {
            name: "RED"
            PropertyChanges { target: rect; color: "red" }
            PropertyChanges { target: mainText; color: "yellow" }
            StateChangeScript { script: Log.info("State changed to RED") }
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
