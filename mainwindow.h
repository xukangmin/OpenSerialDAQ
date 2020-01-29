#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "singlechannel.h"
#include "databasemanager.h"
#include "device.h"
#include "devicedata.h"
#include "dataprocessor.h"
#include <QDebug>

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
    SingleChannel* m_singleChannel;
    QVector<Device*> m_dev_list;

public slots:
    void getData(currentData data, int ch_id);
};
#endif // MAINWINDOW_H
