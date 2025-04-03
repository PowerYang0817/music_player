import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
//Item {

//}

Window{
    id:root
    visible: true
    width: 1300
    height: 950
    title: qsTr("用户登录页面")
//信号声明
    signal loginMessage(string name,string pwd)
    signal newregister(string name,string pwd)
    //页面拖动
    flags: Qt.Window | Qt.FramelessWindowHint //隐藏标题栏
    property int dragx: 0 //属性命令要小写字母开头
    property int dragy: 0
    property bool isDrag: false
    MouseArea{
        width: parent.width
        height: 100
        //鼠标按下
        onPressed: {
            //记录下鼠标按下的坐标，更改标志位
            root.dragx = mouseX;
            root.dragy = mouseY;
            root.isDrag = true
        }
        //鼠标抬起
        onReleased: root.isDrag = false;//鼠标释放
        //位置改变时候
        onPositionChanged: {
            if(root.isDrag)
            {
//                console.log("mouseX = ", mouseX)
//                console.log("mouseY = ",mouseY)
                //拖动的位置 = 鼠标移动后位置-鼠标按下的位置
                root.x += mouseX - root.dragx
                root.y += mouseY - root.dragy
            }
        }
    }


    Rectangle{//背景
        id:rect2
        width: parent.width
        height: parent.height
        gradient: Gradient{
//            GradientStop { position: 0.0; color: "#98FF98" } // 起始颜色
//            GradientStop { position: 0.5; color: "#ADD8E6" } // 中间颜色
//            GradientStop { position: 1.0; color: "#E6E6FA" } // 结束颜色
            GradientStop { position: 0.0; color: "pink" } // 起始颜色
            GradientStop { position: 1.0; color: "sky blue" } // 结束颜色
            orientation:Gradient.Horizontal
        }
    }
    //右上角的x 关闭窗口
    Rectangle{
        //右上角的坐标位置
        x:root.width - 35
        y:5
        width: 30
        height: 30
        color: "#00000000" //rgba 最后表示透明度
        Text {
            id: close
            text: qsTr("×")
            color: "black"
            font.pixelSize: 28
            anchors.centerIn: parent
        }
        //鼠标区域事件
        MouseArea{
            anchors.fill: parent
            //设置监听鼠标移入事件，默认不开启
            hoverEnabled: true
            //鼠标进入时
            onEntered: {
                parent.color = "#1BFFFFFF"
            }
            onExited: parent.color = "#00000000" // 鼠标退出时
            onPressed: parent.color = "#FF0000"; //鼠标按下时
            onClicked: root.close() //调用主窗口函数，关闭界面。
        }
    }
    Rectangle{//内部
        id:rect1
        width: parent.width*0.7
        height: parent.height*0.7
        color: "white"
        radius: 10
        anchors.centerIn: parent
        gradient: Gradient{
            GradientStop { position: 0.0; color: "#FFFACD" } // 起始颜色
            GradientStop { position: 1.0; color: "#C1CDC1" } // 结束颜色
//            orientation:Gradient.Horizontal
        }
        Image {
           id: image
           source: "qrc:/picture/login_qq.jpg"
           width: 370
           height: 370
           x: 50
           y: 150
           // 新增一个属性来标记旋转状态
           property bool isRotating: false
               // 创建一个 SequentialAnimation 作为根动画节点
           SequentialAnimation {
               id: rootAnimation
               running: false
               loops: Animation.Infinite
               RotationAnimation {
                   target: image
                   from: 0
                   to: 360
                   duration: 4000
                   // 固定为顺时针旋转
                   direction: RotationAnimation.Clockwise
               }
           }
           MouseArea {
               anchors.fill: parent
               onClicked: {
                   if (image.isRotating) {
                       // 若正在旋转，暂停动画
                       rootAnimation.running = false
                       image.isRotating = false
                   } else {
                       // 若未旋转，启动动画并设置旋转状态
                       rootAnimation.running = true
                       image.isRotating = true
                   }
               }
           }
        }
        //
        Text {
            id: text1
            text: qsTr("登录系统")
            font.pixelSize: 34
            x:rect1.x+rect1.width*0.40
            y:rect1.y
        }
        TextField{
            objectName: "username_input"
            id:username
            width:rect1.width*0.44
            height: rect1.height*0.1
            //
            placeholderText: "请输入账号"
            x:rect1.x+rect1.width*0.28
            y:rect1.y+rect1.height*0.1;
            background: Rectangle{
                radius: 10
//                color: "#E8E8E8"
            }
        }
        TextField{
            objectName: "userpwd_input"
            id:userpwd
            width:rect1.width*0.44
            height: rect1.height*0.1
            //
            placeholderText: "请输入密码"
            x:rect1.x+rect1.width*0.28
            y:rect1.y+rect1.height*0.23;
            echoMode:TextInput.Password
            background: Rectangle{
                radius: 10
//                color: "#E8E8E8"
            }
        }
        Button{
            id:login
            text: "登录"
            palette.buttonText:"white"
            x:rect1.x+rect1.width*0.32
            y:rect1.y+rect1.height*0.36;
            width: rect1.width*0.15;
            height: rect1.height*0.1;
            font.pixelSize: 34

            onClicked:{
                loginMessage(username.text,userpwd.text);
            }

            background: Rectangle{
                radius: 25
                color: {
                    if(login.down)
                        return "green"
                    else if(login.hovered)
                        return "blue"
                    else
                        return "pink"
                }
            }
        }
        Button{
            id:register
            text: "注册"
            palette.buttonText:"white"
            x:rect1.x+rect1.width*0.54
            y:rect1.y+rect1.height*0.36;
            width: rect1.width*0.15;
            height: rect1.height*0.1;
            font.pixelSize: 34
            //单击
            onClicked:{
                newregister(username.text,userpwd.text);
            }
            background: Rectangle{
                radius: 25
                color: {
                    if(register.down)
                        return "green"
                    else if(register.hovered)
                        return "blue"
                    else
                        return "pink"
                }
            }
        }
    }

}
