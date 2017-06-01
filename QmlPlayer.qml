import QtQuick 2.5
import VideoItem 1.0

Rectangle {
    id: window
    color: "black"
    VideoItem {
        id: videoItem
        width: window.width
        anchors.top: parent.top
        anchors.bottom: buttons.top
        surface: videoSurface
    }
    Row {
        id: buttons
        height: 35
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 5
        Rectangle {
            id: playButton
            width: 60
            height: 25
            border.width: 1
            anchors.verticalCenter: parent.verticalCenter
            Text {
                text: "Старт"
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: videoQML.play()
            }
        }
        Rectangle {
            id: pauseButton
            width: 60
            height: 25
            border.width: 1
            anchors.verticalCenter: parent.verticalCenter
            Text {
                text: "Пауза";
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent;
                onClicked: videoQML.pause()
            }
        }
        Rectangle {
            id: stopButton
            width: 60
            height: 25
            border.width: 1
            anchors.verticalCenter: parent.verticalCenter
            Text {
                text: "Стоп"
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: videoQML.stop()
            }
        }
    }
}
