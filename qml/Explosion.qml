import QtQuick 2.3
import QtQuick.Window 2.2


Item {
    id: explosion
    x: modelData.positionX
    y: modelData.positionY
    width: modelData.width
    height: modelData.height
    
    AnimatedSprite {
        id: explosionSprite
        anchors.centerIn: parent
        width: modelData.width
        height: modelData.height
        source: modelData.objectImagePath
        frameCount: 3
        frameRate: 10
        paused: modelData.animationPaused
        rotation: modelData.rotation
    }
    
    
}
