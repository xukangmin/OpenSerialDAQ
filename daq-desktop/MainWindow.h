#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QVBoxLayout>
#include <QStackedWidget>
#include "DialogNewChannel.h"
#include "WidgetChannel.h"
#include "WidgetChannelList.h"
#include "Device/DeviceModel.h"

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



    // Widgets
    QStackedWidget* mStackedWidget;
    WidgetChannelList* m_widgetChannelList;

    // UI - dialogs
    DialogNewChannel* mDialogNewChannel;

    ChannelModel *channelModel;
    DeviceModel *deviceModel;
public slots:
    void showOverViewPage();
    void showDevicePage();
    void showNewChannelDialog();

private slots:

};
#endif // MAINWINDOW_H
