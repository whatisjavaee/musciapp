import QtQuick 2.9
import QtQuick.Controls 2.2
import FileLinePlayer 1.0
import QtQuick.Window 2.2
import OpenGLUnderQML 1.0
ApplicationWindow {
    visible: true
    width: isMobileDevice( )?Screen.width: 640
    height: isMobileDevice( )?Screen.height:480
    title: qsTr("Scroll")
    /*  ScrollView {
        anchors.fill: parent

        ListView {
            width: parent.width
            model: 20
            delegate: ItemDelegate {
                text: "Item " + (index + 1)
                width: parent.width
            }
        }
    }*/
    //background: "FFFFFF"
    //FileLinePlayer{
    //    id:fPlayer
    //    anchors.fill: parent
    //}

    Squircle {
        SequentialAnimation on t {
            NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
            NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
            loops: Animation.Infinite
            running: true
        }
    }

    function isMobileDevice( )// 判断是否是移动平台
    {
        return  Qt.platform.os === "android" ||
                Qt.platform.os === "blackberry" ||
                Qt.platform.os === "ios" ||
                Qt.platform.os === "winphone";
    }
}
