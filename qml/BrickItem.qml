import QtQuick 2.3
import QtQuick.Window 2.2


Item{
    id: brickItem
    x: modelData.positionX
    y: modelData.positionY
    width: modelData.width
    height: modelData.height
    property int sizeX : 8
    property int sizeY : 8
    Image {
        id: wallImage
        anchors.centerIn: parent
        width: modelData.width
        height: modelData.height
        source: modelData.objectImagePath
        rotation: modelData.rotation
    }
    Grid{
        id: brickGrid
        anchors.fill: parent
        rows: modelData.rows
        columns: modelData.cols


        Repeater{
            model: modelData.part
            Rectangle
            {
                width: sizeX
                height: sizeY
                color: (modelData == 0) ? "transparent" : "black";
            }
        }
    }
    
}
