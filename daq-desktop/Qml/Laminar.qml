import QtQuick 2.13
import QtQuick.Controls 2.13
import QtCharts 2.3
import QtQuick.Window 2.13
import QtQuick.Layouts 1.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Controls.Material 2.13

Rectangle {
    id: page
    width: 1024; height: 800
    color: "#ffffff"

    Material.theme: Material.Dark
    Material.accent: Material.Purple

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
                y: 2
                width: 178
                height: 141

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

                ColumnLayout {
                    spacing: 0

                    Label {
                        id: lbStandardGas
                        text: qsTr("Standard Gas")
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        font.bold: false
                        Layout.preferredWidth: 141
                    }

                    ComboBox {
                        id: cbStandardGas
                        font.strikeout: false
                        hoverEnabled: false
                        currentIndex: 0
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        Layout.preferredWidth: 120
                        model: unitAndConversion.getGasNameList()
                        onActivated: {
                            variableProxyModel.setDataByName("Standard Gas Type",currentText)

                        }
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
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        wheelEnabled: true
                        hoverEnabled: false
                        Layout.fillWidth: true
                        Layout.preferredHeight: 26
                        currentIndex: 0
                        Layout.preferredWidth: 119
                        textRole: "name"
                        model: variableGroupModel
                        onActivated: {
                            widgetStationPapge.setVariableGroupIndex(index)
                            tfStandardFlowrate.text = variableProxyModel.getDataByName("Q,sccm")
                            lbRangeMin.text = variableProxyModel.getDataByName("RangeMinDisplay")
                            lbRangeMax.text = variableProxyModel.getDataByName("RangeMaxDisplay")
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
                        textRole: qsTr("")
                        wheelEnabled: true
                        Layout.fillWidth: true
                        currentIndex: variableProxyModel.getUnitIndexByName("Air Flow Unit")
                        model: unitAndConversion.getUnitNameList("AirFlow")
                        Layout.preferredHeight: 26
                        Layout.preferredWidth: 120
                        onActivated: {
                            variableProxyModel.setDataByName("Air Flow Unit",currentText)
                        }
                    }
                }

                ColumnLayout {
                    Layout.preferredHeight: 52
                    Layout.preferredWidth: 165
                    spacing: 0
                    Label {
                        id: lbGasType
                        text: qsTr("Gas Flowed")
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        font.bold: false
                        Layout.preferredWidth: 141
                    }

                    ComboBox {
                        id: cbGasType
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        Layout.preferredWidth: 120
                        currentIndex: variableProxyModel.getGasTypeIndexByName("Flow Gas Type")
                        model: unitAndConversion.getGasNameList()
                        onActivated: {
                            variableProxyModel.setDataByName("Flow Gas Type",currentText)
                        }
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
                        currentIndex: variableProxyModel.getUnitIndexByName("Temperature Unit")
                        model: unitAndConversion.getUnitNameList("Temperature")
                        onActivated: {
                            variableProxyModel.setDataByName("Temperature Unit",currentText)
                        }
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
                        currentIndex: variableProxyModel.getUnitIndexByName("Pressure Unit")
                        model: unitAndConversion.getUnitNameList("Pressure")
                        onActivated: {
                            variableProxyModel.setDataByName("Pressure Unit",currentText)
                        }
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
                        currentIndex: variableProxyModel.getUnitIndexByName("Delta Pressure Unit")
                        model: unitAndConversion.getUnitNameList("Pressure")
                        onActivated: {
                            variableProxyModel.setDataByName("Delta Pressure Unit",currentText)
                        }
                    }
                }

                ColumnLayout {
                    spacing: 0

                    Label {
                        id: lbSampleSize
                        text: qsTr("Sample Size")
                        Layout.preferredHeight: 26
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        font.bold: false
                        Layout.preferredWidth: 141
                    }

                    SpinBox {
                        id: sbSampleSize
                        Layout.preferredHeight: 34
                        Layout.preferredWidth: 165
                        value: variableProxyModel.getDataByName("Sample Size")
                        onValueChanged: {
                            variableProxyModel.setDataByName("Sample Size",value)
                        }
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
                text: variableProxyModel.getDataByName("StandardFlowRate")
                antialiasing: true
                renderType: Text.NativeRendering
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
                readOnly: true
                Connections {
                    target: variableProxyModel
                    onDataChanged: {
                        tfStandardFlowrate.text = variableProxyModel.getDataByName("StandardFlowRate")
                    }
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
                text: cbFlowUnit.currentText.substring(1,cbFlowUnit.currentText.length)
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
                id: lbAcutalFlowrate
                x: 32
                y: 84
                text: qsTr("ACTUAL FLOWRATE")
                antialiasing: true
                font.pointSize: 18
            }

            TextField {
                id: tfAverageStandardFlowrate
                x: 32
                y: 208
                width: 323
                height: 42
                text: variableProxyModel.getAverageDataByName("StandardFlowRate",sbSampleSize.value)
                antialiasing: true
                font.pointSize: 18
                horizontalAlignment: Text.AlignHCenter
                Connections {
                    target: variableProxyModel
                    onDataChanged: {
                        tfAverageStandardFlowrate.text = variableProxyModel.getAverageDataByName("StandardFlowRate",sbSampleSize.value)
                    }
                }
            }

            TextField {
                id: tfAverageFlowrateUnit
                x: 361
                y: 208
                width: 99
                height: 42
                text: tfStandardFlowrateUnit.text
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
                text: variableProxyModel.getSTDEVDataByName("StandardFlowRate",sbSampleSize.value)
                antialiasing: true
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
                Connections {
                    target: variableProxyModel
                    onDataChanged: {
                        tfAverageStandardFlowrateSTDEV.text = variableProxyModel.getSTDEVDataByName("StandardFlowRate",sbSampleSize.value)
                    }
                }
            }

            TextField {
                id: tfAverageActualFlowrate
                x: 32
                y: 296
                width: 323
                height: 42
                text: variableProxyModel.getAverageDataByName("ActualFlowRate",sbSampleSize.value)
                antialiasing: true
                font.pointSize: 18
                horizontalAlignment: Text.AlignHCenter
                Connections {
                    target: variableProxyModel
                    onDataChanged: {
                        tfAverageActualFlowrate.text = variableProxyModel.getAverageDataByName("ActualFlowRate",sbSampleSize.value)
                    }
                }
            }

            TextField {
                id: tfAverageActualFlowrateUnit
                x: 361
                y: 296
                width: 99
                height: 42
                text: tfActualFlowRateUnit.text
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
                text: variableProxyModel.getSTDEVDataByName("ActualFlowRate",sbSampleSize.value)
                antialiasing: true
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
                Connections {
                    target: variableProxyModel
                    onDataChanged: {
                        tfAverageActualFlowrateSTDEV.text = variableProxyModel.getSTDEVDataByName("ActualFlowRate",sbSampleSize.value)
                    }
                }
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
                text: variableProxyModel.getDataByName("ActualFlowRate")
                antialiasing: true
                font.pointSize: 18
                renderType: Text.NativeRendering
                horizontalAlignment: Text.AlignHCenter
                Connections {
                    target: variableProxyModel
                    onDataChanged: {
                        tfActualFlowRate.text = variableProxyModel.getDataByName("ActualFlowRate")
                    }
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
                    id: lbRangeMin
                    text: variableProxyModel.getDataByName("RangeMinDisplay")
                    Layout.preferredHeight: 24
                    Layout.preferredWidth: 90
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 15
                    font.bold: true
                    Connections {
                        target: variableProxyModel
                        onDataChanged: lbRangeMin.text = variableProxyModel.getDataByName("RangeMinDisplay")
                    }
                }

                Label {
                    id: label3
                    text: qsTr("TO")
                    Layout.preferredWidth: 90
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 15
                }

                Label {
                    id: lbRangeMax
                    text: variableProxyModel.getDataByName("RangeMaxDisplay")
                    Layout.preferredHeight: 24
                    Layout.preferredWidth: 90
                    font.pointSize: 15
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    Connections {
                        target: variableProxyModel
                        onDataChanged: lbRangeMax.text = variableProxyModel.getDataByName("RangeMaxDisplay")
                    }
                }

                Label {
                    id: lbRangeFlowUnit
                    text: cbFlowUnit.currentText
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
            width: 359
            height: 352
            title: qsTr("Data")

            Label {
                id: label12
                x: 0
                y: 75
                text: qsTr("Delta Pressure")
                font.pointSize: 12
            }

            TextField {
                id: tfDeltaPressure
                x: 0
                y: 98
                width: 171
                height: 32
                text: variableProxyModel.getDataByName("Delta Pressure Display")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
                Connections {
                    target: variableProxyModel
                    onDataChanged: tfDeltaPressure.text = variableProxyModel.getDataByName("Delta Pressure Display")
                }
            }

            Label {
                id: label8
                x: 0
                y: 143
                text: qsTr("Pressure")
                font.pointSize: 12
            }

            TextField {
                id: tfPressure
                x: 0
                y: 168
                width: 171
                height: 32
                text: variableProxyModel.getDataByName("Pressure Display")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
                Connections {
                    target: variableProxyModel
                    onDataChanged: tfPressure.text = variableProxyModel.getDataByName("Pressure Display")
                }
            }

            ColumnLayout {
            }

            TextField {
                id: tfDeltaPressureUnit
                x: 177
                y: 98
                width: 105
                height: 32
                text: cbDeltaPressureUnit.currentText
                font.pointSize: 16
                horizontalAlignment: Text.AlignHCenter
                Connections {
                    target: variableProxyModel
                }
            }

            Label {
                id: label7
                x: 0
                y: 14
                text: qsTr("Temperatue")
                font.pointSize: 12
            }

            TextField {
                id: tfTemperature
                x: 0
                y: 37
                width: 171
                height: 32
                text: variableProxyModel.getDataByName("Temperature Display")
                Layout.preferredHeight: 40
                Layout.preferredWidth: 200
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 13
                Connections {
                    target: variableProxyModel
                    onDataChanged: tfTemperature.text = variableProxyModel.getDataByName("Temperature Display")
                }
            }

            TextField {
                id: tfTemperatureUnit
                x: 177
                y: 37
                width: 105
                height: 32
                text: cbTemperatureUnit.currentText
                Layout.preferredHeight: 40
                Layout.preferredWidth: 105
                font.pointSize: 16
                horizontalAlignment: Text.AlignHCenter
                Connections {
                    target: variableProxyModel
                }
            }

            TextField {
                id: tfPressureUnit
                x: 177
                y: 168
                width: 105
                height: 32
                text: cbPressureUnit.currentText
                font.pointSize: 16
                horizontalAlignment: Text.AlignHCenter
                Connections {
                    target: variableProxyModel
                }
            }
        }

        GroupBox {
            id: gbCalibration
            x: 565
            y: 441
            width: 444
            height: 352
            title: qsTr("Calibration")

            Label {
                id: label17
                x: 97
                y: 53
                text: qsTr("TARGET")
                font.pointSize: 12
            }

            Label {
                id: label18
                x: 232
                y: 53
                text: qsTr("STD")
                font.pointSize: 12
            }

            Label {
                id: label19
                x: 348
                y: 54
                text: qsTr("DUT")
                font.pointSize: 12
            }

            RowLayout {
                x: 8
                y: 78

                Label {
                    id: label10
                    text: qsTr("Point 1")
                    font.pointSize: 12
                }

                TextField {
                    id: tfTemperature1
                    text: "10"
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 119
                    Connections {
                        target: variableProxyModel
                    }
                }

                TextField {
                    id: tfTemperature3
                    text: variableProxyModel.getDataByName("Temperature Display")
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 108
                    Connections {
                        target: variableProxyModel
                    }
                }

                TextField {
                    id: tfTemperature4
                    text: variableProxyModel.getDataByName("Temperature Display")
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 108
                    Connections {
                        target: variableProxyModel
                    }
                }
            }

            RowLayout {
                x: 8
                y: 124
                Label {
                    id: label16
                    text: qsTr("Point 2")
                    font.pointSize: 12
                }

                TextField {
                    id: tfTemperature2
                    text: "20"
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 119
                    Connections {
                        target: variableProxyModel
                    }
                }

                TextField {
                    id: tfTemperature5
                    text: variableProxyModel.getDataByName("Temperature Display")
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 108
                    Connections {
                        target: variableProxyModel
                    }
                }

                TextField {
                    id: tfTemperature6
                    text: variableProxyModel.getDataByName("Temperature Display")
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 108
                    Connections {
                        target: variableProxyModel
                    }
                }
            }

            RowLayout {
                x: 8
                y: 168
                Label {
                    id: label20
                    text: qsTr("Point 3")
                    font.pointSize: 12
                }

                TextField {
                    id: tfTemperature7
                    text: "30"
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 119
                    Connections {
                        target: variableProxyModel
                    }
                }

                TextField {
                    id: tfTemperature8
                    text: variableProxyModel.getDataByName("Temperature Display")
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 108
                    Connections {
                        target: variableProxyModel
                    }
                }

                TextField {
                    id: tfTemperature9
                    text: variableProxyModel.getDataByName("Temperature Display")
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 108
                    Connections {
                        target: variableProxyModel
                    }
                }
            }

            RowLayout {
                x: 8
                y: 213
                Label {
                    id: label21
                    text: qsTr("Point 4")
                    font.pointSize: 12
                }

                TextField {
                    id: tfTemperature10
                    text: "40"
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 119
                    Connections {
                        target: variableProxyModel
                    }
                }

                TextField {
                    id: tfTemperature11
                    text: variableProxyModel.getDataByName("Temperature Display")
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 108
                    Connections {
                        target: variableProxyModel
                    }
                }

                TextField {
                    id: tfTemperature12
                    text: variableProxyModel.getDataByName("Temperature Display")
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 108
                    Connections {
                        target: variableProxyModel
                    }
                }
            }

            RowLayout {
                x: 8
                y: 261
                Label {
                    id: label22
                    text: qsTr("Point 5")
                    font.pointSize: 12
                }

                TextField {
                    id: tfTemperature13
                    text: "50"
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 119
                    Connections {
                        target: variableProxyModel
                    }
                }

                TextField {
                    id: tfTemperature14
                    text: variableProxyModel.getDataByName("Temperature Display")
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 108
                    Connections {
                        target: variableProxyModel
                    }
                }

                TextField {
                    id: tfTemperature15
                    text: variableProxyModel.getDataByName("Temperature Display")
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 108
                    Connections {
                        target: variableProxyModel
                    }
                }
            }

            Label {
                id: label23
                x: 147
                y: -5
                text: qsTr("Cal ID: 80000")
                font.pointSize: 12
            }
        }
    }


}



/*##^##
Designer {
    D{i:2;anchors_x:8;anchors_y:6}D{i:15;anchors_x:8;anchors_y:132}D{i:41;anchors_x:200;anchors_y:5}
D{i:59;anchors_x:200;anchors_y:5}D{i:62;anchors_x:200;anchors_y:5}D{i:38;anchors_x:200;anchors_y:5}
}
##^##*/
