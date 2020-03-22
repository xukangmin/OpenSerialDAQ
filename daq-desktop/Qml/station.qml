import QtQuick 2.13
import QtQuick.Controls 2.13
import QtCharts 2.3
import QtQuick.Window 2.13
import QtQuick.Layouts 1.0
import QtQuick.Controls.Universal 2.0

Rectangle {
    id: page
    width: 1024; height: 800
    color: "#ffffff"

    ScrollView {
        id: mainScrollView
        x: 0
        y: 0
        width: page.width
        height: page.height
        anchors.topMargin: 5
        contentHeight: 800
        contentWidth: 1024

        GroupBox {
            id: gbStandardCondition
            width: 189
            height: 182
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            font.pointSize: 10
            Layout.preferredHeight: 115
            Layout.preferredWidth: 143
            font.bold: true
            title: qsTr("Standard Conditions")

            ColumnLayout {
                x: -7
                y: 0
                width: 178
                height: gbStandardCondition.height - 40

                RowLayout {
                    Layout.preferredHeight: 30
                    Layout.preferredWidth: 178
                    spacing: 0

                    Label {
                        id: label1
                        text: qsTr("Pressure")
                        verticalAlignment: Text.AlignVCenter
                        Layout.preferredWidth: 76
                        Layout.fillWidth: false
                        Layout.fillHeight: false
                        font.bold: false
                    }

                    TextField {
                        id: tfStandardPressure
                        text: qsTr("14.7")
                        font.pointSize: 11
                        Layout.maximumWidth: 62
                        Layout.maximumHeight: 30
                        Layout.columnSpan: 2
                        Layout.fillWidth: false
                        Layout.fillHeight: false
                        font.bold: true
                        Layout.preferredWidth: 62
                        Layout.preferredHeight: 30
                    }

                    Label {
                        id: label2
                        text: qsTr("psia")
                        font.pointSize: 10
                        Layout.columnSpan: 2
                        Layout.fillWidth: false
                        Layout.fillHeight: false
                        font.bold: false
                    }
                }

                RowLayout {
                    Layout.preferredHeight: 30
                    Layout.preferredWidth: 178
                    spacing: 0
                    Layout.fillHeight: false
                    Layout.fillWidth: false
                    Label {
                        id: label14
                        text: qsTr("Temperature")
                        verticalAlignment: Text.AlignVCenter
                        Layout.fillWidth: false
                        Layout.fillHeight: false
                        font.bold: false
                        Layout.preferredWidth: 76
                    }

                    TextField {
                        id: tfStandardPressure1
                        text: qsTr("68.0")
                        font.pointSize: 11
                        Layout.preferredHeight: 30
                        Layout.maximumHeight: 30
                        Layout.fillWidth: false
                        Layout.fillHeight: false
                        Layout.maximumWidth: 62
                        font.bold: true
                        Layout.columnSpan: 2
                        Layout.preferredWidth: 62
                    }

                    Label {
                        id: label15
                        text: qsTr("°F")
                        font.pointSize: 10
                        Layout.fillWidth: false
                        Layout.fillHeight: false
                        font.bold: false
                        Layout.columnSpan: 2
                    }
                }

                RowLayout {
                    Layout.preferredHeight: 26
                    Layout.preferredWidth: 178
                    spacing: 0

                    Label {
                        id: lbStandardGs
                        text: qsTr("Standard Gas")
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        font.bold: false
                        Layout.preferredWidth: 141
                    }

                    ComboBox {
                        id: cbStandardGas
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        Layout.preferredWidth: 120
                    }
                }
            }
        }

        GroupBox {
            id: gbSetting
            width: 189
            height: 601
            anchors.top: gbStandardCondition.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5
            font.pointSize: 10
            Layout.preferredHeight: 432
            Layout.preferredWidth: 143
            title: qsTr("Settings")
            font.bold: true

            ColumnLayout {
                x: 0
                y: -5
                width: 165
                height: 565

                ColumnLayout {
                    Layout.preferredHeight: 52
                    Layout.preferredWidth: 165
                    spacing: 0

                    Label {
                        id: lbSubStation
                        text: qsTr("Sub Station")
                        verticalAlignment: Text.AlignVCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: 26
                        font.bold: false
                        onFocusChanged: {
                            console.log("123")
                        }
                    }

                    ComboBox {
                        id: cbSubStation
                        Layout.fillWidth: true
                        Layout.preferredHeight: 26
                        currentIndex: 0
                        Layout.preferredWidth: 119
                        textRole: "name"
                        model: variableGroupModel
                        onActivated: {
                            variableProxyModel.setGroupIndex(index);
                        }
                    }
                }

                ColumnLayout {
                    Layout.preferredHeight: 52
                    Layout.preferredWidth: 165
                    spacing: 0

                    Label {
                        id: lbFlowUnit
                        text: qsTr("Flow Unit")
                        verticalAlignment: Text.AlignVCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: 26
                        font.bold: false
                    }

                    ComboBox {
                        id: cbFlowUnit
                        Layout.fillWidth: true
                        currentIndex: variableProxyModel.getUnitIndexByName("Air Flow Unit")
                        model: unitAndConversion.getUnitNameList("AirFlow")
                        Layout.preferredHeight: 26
                        Layout.preferredWidth: 120
                        onActivated: {
                            variableProxyModel.setDataByName("Air Flow Unit",currentText)
                            tfStandardFlowrateUnit.text = currentText
                        }
                    }
                }

                ColumnLayout {
                    Layout.preferredHeight: 52
                    Layout.preferredWidth: 165
                    spacing: 0
                    Label {
                        id: lbPressureUnit2
                        text: qsTr("Gas Flowed")
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        font.bold: false
                        Layout.preferredWidth: 141
                    }

                    ComboBox {
                        id: cbPressureUnit2
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        Layout.preferredWidth: 120
                    }
                }

                ColumnLayout {
                    Layout.preferredHeight: 52
                    Layout.preferredWidth: 165
                    spacing: 0

                    Label {
                        id: lbTemperatureUnit
                        text: qsTr("Temperature Unit")
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        Layout.preferredHeight: 26
                        Layout.preferredWidth: 141
                        font.bold: false
                    }

                    ComboBox {
                        id: cbTemperatureUnit
                        Layout.fillWidth: true
                        Layout.preferredHeight: 26
                        Layout.preferredWidth: 120
                    }
                }

                ColumnLayout {
                    Layout.preferredHeight: 52
                    Layout.preferredWidth: 165
                    spacing: 0
                    Label {
                        id: lbPressureUnit
                        text: qsTr("Pressure Unit")
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        font.bold: false
                        Layout.preferredWidth: 141
                    }

                    ComboBox {
                        id: cbPressureUnit
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        Layout.preferredWidth: 120
                    }
                }

                ColumnLayout {
                    Layout.preferredHeight: 52
                    Layout.preferredWidth: 165
                    spacing: 0
                    Label {
                        id: lbDeltaPressureUnit
                        text: qsTr("Delta Pressure Unit")
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        font.bold: false
                        Layout.preferredWidth: 141
                    }

                    ComboBox {
                        id: cbDeltaPressureUnit
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        Layout.preferredWidth: 120
                    }
                }

                ColumnLayout {
                    spacing: 0

                    Label {
                        id: lbPressureUnit3
                        text: qsTr("Sample Size")
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        font.bold: false
                        Layout.preferredWidth: 141
                    }

                    SpinBox {
                        id: spinBox
                        Layout.preferredHeight: 34
                        Layout.preferredWidth: 165
                    }
                }

                ColumnLayout {
                    spacing: 0

                    Label {
                        id: lbPressureUnit4
                        text: qsTr("Interval")
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        font.bold: false
                        Layout.preferredWidth: 141
                    }

                    SpinBox {
                        id: spinBox1
                        Layout.preferredHeight: 34
                        Layout.preferredWidth: 165
                    }
                }
            }



        }

        GroupBox {
            id: gbResult
            width: 809
            height: 383
            antialiasing: true
            scale: 1
            font.bold: false
            anchors.left: gbStandardCondition.right
            anchors.leftMargin: 6
            anchors.top: gbRange.bottom
            anchors.topMargin: 6
            Layout.preferredHeight: 377
            Layout.preferredWidth: 607
            wheelEnabled: false
            visible: true
            title: qsTr("Result")

            TextField {
                id: tfStandardFlowrate
                x: 32
                y: 36
                width: 323
                height: 42
                text: variableProxyModel.getDataByName("Q,sccm")
                antialiasing: true
                renderType: Text.NativeRendering
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
                readOnly: true
                Connections {
                    target: variableProxyModel
                    onDataChanged: tfStandardFlowrate.text = variableProxyModel.getDataByName("Q,sccm")
                }
                onFocusChanged:{
                    if(focus)
                        selectAll()
                }
            }

            TextField {
                id: tfStandardFlowrateUnit
                x: 361
                y: 35
                width: 99
                height: 42
                text: cbFlowUnit.currentText
                antialiasing: true
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
                readOnly: true
                onFocusChanged:{
                    if(focus)
                        selectAll()
                }
            }

            TextField {
                id: tfStandardFlowrateUn
                x: 542
                y: 35
                width: 194
                height: 42
                readOnly: true
                text: qsTr("12345")
                antialiasing: true
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
            }

            TextField {
                id: tfActualFlowRateUnit
                x: 361
                y: 115
                width: 99
                height: 42
                text: qsTr("cfh")
                antialiasing: true
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
            }

            TextField {
                id: tfActualFlowrateUn
                x: 542
                y: 115
                width: 194
                height: 42
                text: qsTr("12345")
                antialiasing: true
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
            }

            Label {
                id: lbStandardFlowrate
                x: 32
                y: 0
                text: qsTr("STANDARD FLOWRATE")
                antialiasing: true
                font.pointSize: 18
            }

            Label {
                id: label10
                x: 32
                y: 84
                text: qsTr("ACTUAL FLOWRATE")
                antialiasing: true
                font.pointSize: 18
            }

            TextField {
                id: tfAverageFlowrate
                x: 32
                y: 208
                width: 323
                height: 42
                text: qsTr("12345.23")
                antialiasing: true
                font.pointSize: 18
                horizontalAlignment: Text.AlignHCenter
            }

            TextField {
                id: rfAverageFlowrateUnit
                x: 361
                y: 208
                width: 99
                height: 42
                text: qsTr("scfh")
                antialiasing: true
                font.pointSize: 18
                horizontalAlignment: Text.AlignHCenter
            }

            TextField {
                id: tfAverageStandardFlowrateSTDEV
                x: 542
                y: 208
                width: 194
                height: 42
                text: qsTr("12345")
                antialiasing: true
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
            }

            TextField {
                id: tfAverageActualFlowrate
                x: 32
                y: 296
                width: 323
                height: 42
                text: qsTr("Text Field")
                antialiasing: true
                font.pointSize: 18
                horizontalAlignment: Text.AlignHCenter
            }

            TextField {
                id: tfAverageActualFlowrateUnit
                x: 361
                y: 296
                width: 99
                height: 42
                text: qsTr("cfh")
                antialiasing: true
                font.pointSize: 18
                horizontalAlignment: Text.AlignHCenter
            }

            TextField {
                id: tfAverageActualFlowrateSTDEV
                x: 542
                y: 296
                width: 194
                height: 42
                text: qsTr("12345")
                antialiasing: true
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
            }

            Label {
                id: lbAverageFlowrate
                x: 32
                y: 173
                text: qsTr("AVERAGE STANDARD FLOWRATE")
                antialiasing: true
                font.pointSize: 18
            }

            Label {
                id: label13
                x: 32
                y: 265
                text: qsTr("AVERAGE ACTUAL FLOWRATE")
                antialiasing: true
                font.pointSize: 18
            }

            Label {
                id: label9
                x: 542
                y: 0
                text: qsTr("UNCERTAINTY")
                antialiasing: true
                font.pointSize: 18
            }

            Label {
                id: label11
                x: 542
                y: 168
                text: qsTr("STD DEVIATION")
                antialiasing: true
                font.pointSize: 18
            }

            TextField {
                id: tfActualFlowRate
                x: 33
                y: 115
                width: 323
                height: 42
                text: qsTr("Text Field")
                antialiasing: true
                font.pointSize: 18
                renderType: Text.NativeRendering
                horizontalAlignment: Text.AlignHCenter
            }

            Button {
                id: button
                x: 745
                y: 0
                width: 45
                height: 29
                text: qsTr("+")
                onClicked: {
                    gbRange.visible = false
                    gbStandardCondition.visible = false
                    gbSetting.visible = false
                    gbData.visible = false

                    gbResult.x = -20
                    gbResult.y = 60


                    var curWidth = gbResult.width
                    var curHeight = gbResult.height

                    var scaleRatio = 1024 / curWidth


                    gbResult.scale = scaleRatio

                    gbResult.width = 1024
                    gbResult.height = 600
                }
            }
        }

        GroupBox {
            id: gbRange
            width: 810
            height: 37
            anchors.left: gbStandardCondition.right
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            title: qsTr("")

            RowLayout {
                x: 115
                y: -5
                width: 544
                height: 24

                Label {
                    id: label
                    text: qsTr("RANGE: ")
                    font.bold: true
                    font.pointSize: 15
                }

                Label {
                    id: label4
                    text: qsTr("0")
                    Layout.preferredHeight: 24
                    Layout.preferredWidth: 90
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 15
                    font.bold: true
                }

                Label {
                    id: label3
                    text: qsTr("TO")
                    Layout.preferredWidth: 90
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 15
                }

                Label {
                    id: label5
                    text: qsTr("3000")
                    Layout.preferredHeight: 24
                    Layout.preferredWidth: 90
                    font.pointSize: 15
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                }

                Label {
                    id: label6
                    text: qsTr("scfh")
                    Layout.preferredHeight: 24
                    Layout.preferredWidth: 90
                    font.pointSize: 15
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }

        GroupBox {
            id: gbData
            x: 200
            y: 441
            width: 809
            height: 352
            title: qsTr("Data")

            Column {
            }

            GridLayout {
                x: 136
                y: 36
                width: 452
                height: 245
                rows: 2
                columns: 2

                ColumnLayout {

                    Label {
                        id: label7
                        text: qsTr("Temperatue")
                        font.pointSize: 15
                    }

                    TextField {
                        id: tfTemperature
                        text: qsTr("Text Field")
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 16
                    }
                }

                ColumnLayout {

                    Label {
                        id: label12
                        text: qsTr("Delta Pressure")
                        font.pointSize: 15
                    }

                    TextField {
                        id: tfDeltaPressure
                        text: qsTr("Text Field")
                        font.pointSize: 16
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                ColumnLayout {

                    Label {
                        id: label8
                        text: qsTr("Pressure")
                        font.pointSize: 15
                    }

                    TextField {
                        id: tfPressure
                        text: qsTr("Text Field")
                        font.pointSize: 16
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                Item {
                    id: spacer
                    Layout.preferredHeight: 14
                    Layout.preferredWidth: 14
                }
            }
        }
    }


}



/*##^##
Designer {
    D{i:2;anchors_x:8;anchors_y:6}D{i:15;anchors_x:8;anchors_y:132}D{i:41;anchors_x:200;anchors_y:5}
D{i:62;anchors_x:200;anchors_y:5}
}
##^##*/
