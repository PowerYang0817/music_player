import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0 //渐变色需要导入的库
ApplicationWindow{
    id:mainWindow
    visible: true
    width: 540
    height: 380
    title: "注册成功！"
    //opacity: 1 // 初始完全不透明
    flags: Qt.Window | Qt.FramelessWindowHint //属性，标题栏隐藏 无框窗口提示,必须要前面的,不然系统桌面菜单栏,看不到软件
    property int dragx: 0 //属性命令要小写字母开头
    property int dragy: 0
    property bool isDrag: false
    Rectangle{
        objectName: "rect"
        radius: 10 //设置主窗口圆角
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        gradient: Gradient{ //设置渐变色，渐变色优先级大于color颜色
        GradientStop{position: 0 ;color: "#4158d0" }
        GradientStop{position: 1 ;color: "#c850c0" }
        orientation: Gradient.Horizontal // 设置方向
        }
        Text { //文本说明：登录系统
            objectName:"tip"
            anchors.centerIn: parent
            width: 120
            height: 30
            font.pointSize: 22
            text: qsTr("注册成功！") // 后续可以进行翻译
            color: "white"
        }
    }
    Timer {
        id: timer
        interval: 1000 // 3秒
        running: true
        onTriggered: {
        fadeOutAnimation.start(); // 开始动画
        }
    }
    SequentialAnimation {
        id: fadeOutAnimation
        NumberAnimation { target: mainWindow; property: "opacity"; to: 0;duration: 1000 } // 渐变到透明需要1秒时间
        PropertyAction { target: mainWindow; property: "visible"; value:false } // 设置visible为false，彻底隐藏窗口
    }

}
