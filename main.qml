import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import OpenGLUnderQML 1.0
ApplicationWindow {
    visible: true
    width: isMobileDevice( )?Screen.width: 800
    height: isMobileDevice( )?Screen.height:400
    title: qsTr("test")
    color:Qt.rgba(0,0,0,0)

    Squircle {
        id: sq
        SequentialAnimation on t {
            NumberAnimation { to: 1; duration: 500000; easing.type: Easing.Linear }
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
