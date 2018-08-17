import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import OpenGLUnderQML 1.0
ApplicationWindow {
    visible: true
    width: isMobileDevice( )?Screen.width: 1000
    height: isMobileDevice( )?Screen.height:600
    title: qsTr("test")
    Squircle {
        id:sq
        sp:20
        anchors.top: parent.top;
        width: parent.width
        height: parent.height/10*9
    }
    Rectangle{
        color:Qt.rgba(0.95,0.95,0.95,1);
        anchors.top: sq.bottom;
        width: parent.width;
        height: parent.height/10
        Button{
            id:start
            anchors.left: parent.left
            text:"开始"
            onClicked: sq.start()
        }
        Button{
            id:stop
            anchors.left: start.right
            text:"停止"
            onClicked: sq.stop()
        }
        Button{
            id:js
            anchors.left: stop.right
            text:"加速"
            onClicked: {sq.sp = sq.sp+1;}
        }
        Button{
            id:jians
            anchors.left: js.right
            text:"减速"
            onClicked: {sq.sp = sq.sp-1;}
        }
        Button{
            id:yf
            anchors.left: jians.right
            text:"音符"
        }
        Button{
            id:fk
            anchors.left: yf.right
            text:"方块"
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
