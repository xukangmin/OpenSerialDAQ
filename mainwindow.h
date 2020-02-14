#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "singlechannel.h"
#include "databasemanager.h"
#include "device.h"
#include "devicedata.h"
#include "dataprocessor.h"
#include <QDebug>
#include <QVBoxLayout>
#include "newchannel.h"
#include "channelwidget.h"
#include "devicewidget.h"
#include "dialogadddevicetochannel.h"
#include "minidevicewidget.h"

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
    NewChannel* m_dlgNewChennel;
    DialogAddDeviceToChannel* m_dlgAddDeviceToChannel;

    QVector<SingleChannel*> m_Channels;
    QVector<Device*> m_dev_list;
    QVector<ChannelWidget*> m_ChannelWidgets;
    QVector<DeviceWidget*> m_DeviceWidgets;
    QVector<Channel> m_channle_list;

    QVBoxLayout* scrollVLayout;

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
