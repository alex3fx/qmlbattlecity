import QtQuick 2.3
import QtQuick.Window 2.2


Item {
    id: userTank
    x: modelData.positionX
    y: modelData.positionY
    width: modelData.width
    height: modelData.height
    
    AnimatedSprite {
        id: userTankSprite
        anchors.centerIn: parent
        width: modelData.width
        height: modelData.height
        source: modelData.objectImagePath
        frameCount: 2
        frameRate: 10
        paused: modelData.animationPaused
        rotation: modelData.rotation
    }
    

}
