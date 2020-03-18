import QtQuick 2.0
import QtQuick.Controls 2.13
import QtCharts 2.3
import QtQuick.Window 2.13
import QtQuick.Layouts 1.0
import QtQuick.Controls.Universal 2.0

Rectangle {
    id: page
    width: 800; height: 600
    color: "#ffffff"

    ColumnLayout {
        x: 18
        y: 22
        Layout.preferredHeight: 557
        Layout.preferredWidth: 141

        GroupBox {
            id: groupBox
            Layout.preferredHeight: 115
            Layout.preferredWidth: 143
            font.bold: true
            title: qsTr("Standard Conditions")

            ColumnLayout {
                x: -12
                y: 11
                width: 143
                height: 92
                RowLayout {
                    Label {
                        id: label1
                        text: qsTr("Pressure")
                        font.bold: false
                    }

                    TextField {
                        id: textField1
                        text: qsTr("14.7")
                        font.bold: false
                        Layout.preferredWidth: 71
                        Layout.preferredHeight: 28
                    }
                }

                RowLayout {

                    Label {
                        id: label
                        text: qsTr("Temperature")
                        font.bold: false
                    }

                    TextField {
                        id: textField
                        text: qsTr("68.0")
                        font.bold: false
                        Layout.preferredHeight: 28
                        Layout.preferredWidth: 71
                    }
                }
            }
        }

        GroupBox {
            id: groupBox1
            Layout.preferredHeight: 432
            Layout.preferredWidth: 143
            title: qsTr("Settings")
            font.bold: true

            ColumnLayout {
                y: 10
                width: groupBox1.availableWidth
                height: groupBox1.availableHeight
                transformOrigin: Item.Center

                Label {
                    id: label2
                    text: qsTr("Flow")
                    font.bold: false
                }

                ComboBox {
                    id: comboBox
                    currentIndex: 0
                    model: ListModel {
                        id: model
                        ListElement { text: "500" }
                        ListElement { text: "1000" }
                        ListElement { text: "2000" }
                        ListElement { text: "5000" }
                        ListElement { text: "10000" }
                        ListElement { text: "25000" }
                    }
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 101
                }

                Label {
                    id: label3
                    text: qsTr("Temperature")
                    font.bold: false
                }

                ComboBox {
                    id: comboBox1
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 101
                }

                Label {
                    id: label4
                    text: qsTr("Pressure")
                    font.bold: false
                }

                ComboBox {
                    id: comboBox2
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 101
                }

                Label {
                    id: label5
                    text: qsTr("Delta Pressure")
                    font.bold: false
                }

                ComboBox {
                    id: comboBox3
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 101
                }

                Label {
                    id: label6
                    text: qsTr("Gas Flowed")
                    font.bold: false
                }

                ComboBox {
                    id: comboBox4
                    Layout.preferredWidth: 101
                    Layout.preferredHeight: 20
                }

                Label {
                    id: label7
                    text: qsTr("Standard Gas")
                    font.bold: false
                }

                ComboBox {
                    id: comboBox5
                    Layout.preferredWidth: 101
                    Layout.preferredHeight: 20
                }

                Label {
                    id: label9
                    text: qsTr("Sample Size")
                    font.bold: false
                }

                ComboBox {
                    id: comboBox6
                    Layout.preferredWidth: 101
                    Layout.preferredHeight: 20
                }

                Label {
                    id: label11
                    text: qsTr("Interval")
                    font.bold: false
                }

                ComboBox {
                    id: comboBox7
                    Layout.preferredWidth: 101
                    Layout.preferredHeight: 20
                }
            }
        }
    }

    ColumnLayout {
        x: 167
        y: 31

        GroupBox {
            id: groupBox2
            Layout.preferredHeight: 377
            Layout.preferredWidth: 607
            wheelEnabled: false
            visible: true
            title: qsTr("Result")

            TextField {
                id: textField2
                x: 32
                y: 43
                width: 234
                height: 42
                text: qsTr("12345.23")
                renderType: Text.NativeRendering
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
                readOnly: true
                onFocusChanged:{
                   if(focus)
                       selectAll()
              }
            }

            TextField {
                id: textField3
                x: 272
                y: 43
                width: 99
                height: 42
                text: qsTr("scfh")
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
                readOnly: true
                onFocusChanged:{
                   if(focus)
                       selectAll()
              }
            }

            TextField {
                id: textField4
                x: 377
                y: 43
                width: 194
                height: 42
                readOnly: true
                text: qsTr("12345")
                font.pointSize: 18
            }

            TextField {
                id: textField5
                x: 32
                y: 123
                width: 234
                height: 42
                text: qsTr("Text Field")
                renderType: Text.NativeRendering
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
            }

            TextField {
                id: textField6
                x: 272
                y: 123
                width: 99
                height: 42
                text: qsTr("cfh")
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
            }

            TextField {
                id: textField7
                x: 377
                y: 123
                width: 194
                height: 42
                text: qsTr("12345")
                font.pointSize: 18
            }

            Label {
                id: label8
                x: 32
                y: 8
                text: qsTr("STANDARD FLOWRATE")
                font.pointSize: 18
            }

            Label {
                id: label10
                x: 32
                y: 92
                text: qsTr("ACTUAL FLOWRATE")
                font.pointSize: 18
            }

            TextField {
                id: textField8
                x: 32
                y: 216
                width: 234
                height: 42
                text: qsTr("12345.23")
                font.pointSize: 18
                horizontalAlignment: Text.AlignHCenter
            }

            TextField {
                id: textField9
                x: 272
                y: 216
                width: 99
                height: 42
                text: qsTr("scfh")
                font.pointSize: 18
                horizontalAlignment: Text.AlignHCenter
            }

            TextField {
                id: textField10
                x: 377
                y: 216
                width: 194
                height: 42
                text: qsTr("12345")
                font.pointSize: 18
            }

            TextField {
                id: textField11
                x: 32
                y: 304
                width: 234
                height: 42
                text: qsTr("Text Field")
                font.pointSize: 18
                horizontalAlignment: Text.AlignHCenter
            }

            TextField {
                id: textField12
                x: 272
                y: 304
                width: 99
                height: 42
                text: qsTr("cfh")
                font.pointSize: 18
                horizontalAlignment: Text.AlignHCenter
            }

            TextField {
                id: textField13
                x: 377
                y: 304
                width: 194
                height: 42
                text: qsTr("12345")
                font.pointSize: 18
            }

            Label {
                id: label12
                x: 32
                y: 181
                text: qsTr("AVERAGE STANDARD FLOWRATE")
                font.pointSize: 18
            }

            Label {
                id: label13
                x: 32
                y: 273
                text: qsTr("AVERAGE ACTUAL FLOWRATE")
                font.pointSize: 18
            }
        }

        GroupBox {
            id: groupBox3
            Layout.preferredHeight: 153
            Layout.preferredWidth: 607
            title: qsTr("Data")
        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
