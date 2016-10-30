import QtQuick 2.3
import QtQuick.Window 2.2


Item {
    id: bullet
    x: modelData.positionX
    y: modelData.positionY
    width: modelData.width
    height: modelData.height
    
    Image {
        id: bulletImage
        anchors.centerIn: parent
        width: modelData.width
        height: modelData.height
        source: modelData.objectImagePath
        rotation: modelData.rotation
    }
    
    
}
