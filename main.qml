import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import OpenGLUnderQML 1.0
ApplicationWindow {
    visible: true
    width: isMobileDevice( )?Screen.width: 1920/2
    height: isMobileDevice( )?Screen.height:1080/2
    title: qsTr("test")
    Squircle {
        id:sq
        sp:60
        anchors.top: parent.top;
        width: parent.width
        anchors.bottom: start.top
    }
    Grid{
        anchors.right: parent.right
        rows:1
        columns: 8
        columnSpacing: 4;
        flow:Grid.TopToBottom
        Text{
            font.pixelSize: 30
            text: "速度:"
        }
        Text{
            id:sd
            font.pixelSize: 30
            text: sq.sp
        }
        Text{
            font.pixelSize: 30
            text: "得分:"
        }
        Text{
            id:df
            font.pixelSize: 30
            text: "0"
        }
        Text{
            font.pixelSize: 30
            text: "连击:"
        }
        Text{
            id:lj
            text: "0"
            font.pixelSize: 30
        }
        Text{
            font.pixelSize: 30
            text: "错误:"
        }
        Text{
            id:cw
            text: "0"
            font.pixelSize: 30
        }
    }
    Button{
        id:start
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        text:"开始"
        onClicked: sq.start()
    }
    Button{
        id:stop
        anchors.left: start.right
        anchors.top: start.top
        text:"停止"
        onClicked: sq.stop()
    }
    Button{
        id:js
        anchors.left: stop.right
        anchors.top: start.top
        text:"加速"
        onClicked: {sq.sp = sq.sp+1;sd.text=sq.sp}
    }
    Button{
        id:jians
        anchors.left: js.right
        anchors.top: start.top
        text:"减速"
        onClicked: {sq.sp = sq.sp-1;sd.text=sq.sp}
    }
    Button{
        id:yf
        anchors.left: jians.right
        anchors.top: start.top
        text:"音符"
    }
    Button{
        id:fk
        anchors.left: yf.right
        anchors.top: start.top
        text:"方块"
    }
    Button{
        id:rt
        anchors.left: fk.right
        anchors.top: start.top
        text:"返回"
    }
    function isMobileDevice( )// 判断是否是移动平台
    {
        return  Qt.platform.os === "android" ||
                Qt.platform.os === "blackberry" ||
                Qt.platform.os === "ios" ||
                Qt.platform.os === "winphone";
    }
    Binding{
        target: lj
        property: "text"
        value:sq.rightNumber
    }
    Binding{
        target: cw
        property: "text"
        value:sq.errorNum
    }
}
