import QtQuick 2.0
import QtQuick.Controls 2.13
import QtCharts 2.3
import QtQuick.Window 2.13
import QtQuick.Layouts 1.0
import QtQuick.Controls.Universal 2.0

Rectangle {
    id: page
    width: 300; height: 200
    color: "#ffffff"
    ScrollView {
        id: scrollView
        x: 0
        y: 0
        width: page.width
        height: page.height
        contentHeight: 800
        contentWidth: 1024

        Column {
            id: gLayout
            width: page.width
            height: page.height
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            onHeightChanged: {
                gLayout.scale = gLayout.height / 300
            }

            RowLayout {

                Label {
                    id: label
                    text: qsTr("Label")
                    Layout.preferredHeight: 48
                    Layout.preferredWidth: 95
                    font.pointSize: 20
                }

                Button {
                    id: button1
                    text: qsTr("Button")
                    Layout.preferredHeight: 41
                    Layout.preferredWidth: 116
                    onClicked: {
                        if (glayout.scale == 2)
                            glayout.scale = 1
                        else
                            glayout.scale = 2
                    }
                }
            }

            RowLayout {

                TextField {
                    id: textField
                    text: qsTr("Text Field")
                    Layout.preferredHeight: 40
                    Layout.preferredWidth: 120
                    Layout.columnSpan: 2
                }

                Button {
                    id: button
                    text: qsTr("Button")
                    Layout.preferredHeight: 41
                    Layout.preferredWidth: 116
                }
            }
        }
    }

}



/*##^##
Designer {
    D{i:2;anchors_x:0;anchors_y:0}D{i:1;anchors_y:31}
}
##^##*/
