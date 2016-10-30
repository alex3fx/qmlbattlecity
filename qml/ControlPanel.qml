import QtQuick 2.3
import QtQuick.Window 2.2

Item {
    id: controlPanel
    Item {
        id: controlUpperPanel
        anchors.top: parent.top
        anchors.left: parent.left
        Grid {
            id: controlPanelGrid
            columns: 2
            rows: gameBoard.maxFragsCount / 2
            spacing: 0
            Repeater {
                model: gameBoard.frags
                Item {
                    id: tile
                    width: modelData.width
                    height: modelData.height

                    Image {
                        id: tileBackground
                        anchors.centerIn: parent
                        width: modelData.width
                        height: modelData.height
                        source: modelData.objectImagePath
                        rotation: modelData.rotation
                    }
                }
            }
        }
    }
    Item {
        id: controlLowerPanel
        y: controlPanelGrid.y + 200
        anchors.left: parent.left
        Grid {
            id: controlLowerPanelGrid
            columns: 1
            rows: 4
            spacing: 0
            Row {
                Text {
                    font.family: "Helvetica"
                    font.pointSize: 20
                    smooth: true
                    color: "black"
                    text: "IP"
                }
            }
            Row {
                Text {
                    font.family: "Helvetica"
                    font.pointSize: 20
                    smooth: true
                    color: "black"
                    text: gameBoard.fragsCount
                }
            }
            Row {
                Image {
                    id: flagsImage
                    source: "qrc:/img/flag.png"
                }
            }
            Row {
                Text {
                    font.family: "Helvetica"
                    font.pointSize: 20
                    smooth: true
                    color: "black"
                    text: gameBoard.lifesCount
                }
            }
        }
    }
}
