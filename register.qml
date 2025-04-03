import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12


ApplicationWindow{
    id:root
    visible: true
    width: 1100
    height: 800
    title: qsTr("用户注册页面")
    signal registerMessage(string name_in,string pwd_in)
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
           source: "qrc:/picture/signup.jpg"
           width: 320
           height: 320
           x: 30
           y: 100
        }
        Text {
            id: text
            text: qsTr("注册页面")
            font.pixelSize: 34
            x:root.width*0.43
            y:root.height*0.1
        }

        TextField{
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
                registerMessage(username.text,userpwd.text);
                root.close()
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
