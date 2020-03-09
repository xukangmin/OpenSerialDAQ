#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QVBoxLayout>
#include <QStackedWidget>
#include "DialogNewChannel.h"
#include "WidgetChannel.h"
#include "WidgetChannelList.h"
#include "WidgetDevicePage.h"
#include "Device/DeviceModel.h"
#include "Models.h"

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
    WidgetDevicePage* mWidgetDevicePage;

    // UI - dialogs
    DialogNewChannel* mDialogNewChannel;

    Models& allModels;

public slots:
    void showOverViewPage();
    void showDevicePage();
    void showNewChannelDialog();
    void triggerTestButton1();
    void triggerTestButton2();
    void triggerTestButton3();

private slots:

};
#endif // MAINWINDOW_H
