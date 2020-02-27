#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ThreadChannel.h"
#include "DatabaseManager.h"
#include "Device.h"
#include "DeviceData.h"
#include "ThreadDataProcessor.h"
#include <QDebug>
#include <QVBoxLayout>
#include "DialogNewChannel.h"
#include "WidgetChannel.h"
#include "WidgetDevice.h"
#include "DialogAddDeviceToChannel.h"
#include "WidgetMiniDevice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void updateData();
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // UI - dialogs
    DialogNewChannel* m_dlgNewChennel;
    DialogAddDeviceToChannel* m_dlgAddDeviceToChannel;

    QVector<ThreadChannel*> m_Channels;
    QVector<Device*> m_dev_list;
    QVector<ChannelWidget*> m_ChannelWidgets;
    QVector<DeviceWidget*> m_DeviceWidgets;
    QVector<Channel> m_channle_list;

    QVBoxLayout* scrollVLayout;
    void createNewChannel(Channel ch_info);

public slots:
    void getData(QVector<DeviceData> data, int ch_id);
    void showOverViewPage();
    void showDevicePage();
    void showNewChannelDialog();
    void showAddDeviceToChannelDialog(int id);
    void deleteChannel(int id);
    //void deviceSelected(QModelIndex index);

private slots:

};
#endif // MAINWINDOW_H
