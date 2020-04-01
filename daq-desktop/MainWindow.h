#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QSettings>
#include "DialogNewChannel.h"
#include "WidgetChannel.h"
#include "WidgetChannelListPage.h"
#include "WidgetDevicePage.h"
#include "Device/DeviceModel.h"
#include "DialogNewDevice.h"
#include "Models.h"
#include "WidgetSettingPage.h"
#include "WidgetStationPage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QSettings mSettings;

    Models& allModels;

    QTimer* mTimer_test;

    // Widgets
    QStackedWidget* mStackedWidget;
    WidgetChannelListPage* mWidgetChannelListPage;
    WidgetDevicePage* mWidgetDevicePage;
    WidgetStationPage* mWidgetStationPage;
    WidgetSettingPage* mWidgetSettingPage;

    // UI - dialogs
    DialogNewChannel* mDialogNewChannel;
    DialogNewDevice* mDialogNewDevice;




    void setupCommonToolBar();
    void setupTestToolBar();




public slots:
    void showChannelPage();
    void showDevicePage();
    void showStationPage();
    void showSettingPage();

    void showNewChannelDialog();
    void showNewDeviceDialog();

    void startAllChannels(bool isChecked);
    void stopAllChannels();

    void toggleSimulation(bool isChecked);

    void triggerTestButton1();
    void triggerTestButton2();
    void triggerTestButton3();
    void triggerTestButton4();


private slots:

};
#endif // MAINWINDOW_H
