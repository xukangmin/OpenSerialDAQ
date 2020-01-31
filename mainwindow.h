#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "singlechannel.h"
#include "databasemanager.h"
#include "device.h"
#include "devicedata.h"
#include "dataprocessor.h"
#include <QDebug>
#include "newchannel.h"
#include "channelwidget.h"

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

    QVector<SingleChannel*> m_Channels;
    QVector<Device*> m_dev_list;
    QVector<ChannelWidget*> m_ChannelWidgets;


public slots:
    void getData(QVector<DeviceData> data, int ch_id);
    void showOverViewPage();
    void showDevicePage();
    void showNewChannelDialog();
    void deleteChannel(int id);

};
#endif // MAINWINDOW_H
