import QtQuick
import QtQuick.Controls 2.15
import local.myhost 1.0 // from main.cpp

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("QtDBus-demo")

    DemoServer {
        id: server
    }

    Button {
        id: button
        width: 100
        height: 50
        text: "Reset"
        onClicked: server.reset()
    }

    Text {
        anchors.centerIn: parent
        width: 100
        height: 100
        font.pixelSize: 24
        text: "count=%1".arg(server.count)
    }
}
