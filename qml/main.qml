import QtQuick 2.3
import QtQuick.Window 2.2


Window {
    visible: true
    width: gameBoard.width +100
    height: gameBoard.height + 100
    color: "grey"
    MouseArea {
        anchors.fill: parent
        focus: true
        Item {
            x:30
            y:50
            Rectangle
            {
                id: background
                x: 0
                y: 0
                width: gameBoard.width
                height: gameBoard.height
                color: "black"
            }
            Repeater {
                model: gameBoard.brick
                BrickItem {
                    id: brickItem
                }
            }
            Repeater {
                model: gameBoard.concrete
                ConcreteItem {
                    id: concreteItem
                }
            }
            Repeater {
                model: gameBoard.base
                Base {
                    id: base
                }
            }

            Repeater {
                model: gameBoard.bullet
                Bullet {
                    id: bullet
                }
            }
            Repeater {
                model: gameBoard.player
                UserTank {
                    id: userTank
                }
            }
            Repeater {
                model: gameBoard.enemy
                UserTank {
                    id: enemyTank
                }
            }
            Repeater {
                model: gameBoard.explosion
                Explosion {
                    id: explosion
                }
            }
            Text
            {
                anchors.centerIn: background
                font.family: "Helvetica"
                font.pointSize: 20
                smooth: true
                color: "white"
                text: gameBoard.message
            }
        }
        Keys.onPressed: {
            if (event.key === Qt.Key_Right) {
                gameBoard.direction = 0;
                event.accepted = true;
            }
            else if (event.key === Qt.Key_Down) {
                gameBoard.direction = 1;
                event.accepted = true;
            }
            else if (event.key === Qt.Key_Left) {
                gameBoard.direction = 2;
                event.accepted = true;
            }
            else if (event.key === Qt.Key_Up) {
                gameBoard.direction = 3;
                event.accepted = true;
            }
            else if (event.key === Qt.Key_Space) {
                gameBoard.controlKey = Qt.Key_Space
                event.accepted = true;
            }
        }

        Keys.onReleased: {
            gameBoard.direction = 4;
            gameBoard.controlKey = Qt.Key_unknown
            event.accepted = true;
        }
        ControlPanel {
            id: controlPanel
            x: gameBoard.width + 30
            y: 50
            height: gameBoard.height
            width: 70

        }

    }

}




