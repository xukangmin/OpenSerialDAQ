#ifndef WIDGETSTATIONPAGE_H
#define WIDGETSTATIONPAGE_H

#include <QWidget>
#include <Variable/VariableModel.h>
#include <QQuickWidget>
#include "UnitAndConversion.h"
namespace Ui {
class WidgetStationPage;
}

class WidgetStationPage : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetStationPage(QWidget *parent = nullptr);
    ~WidgetStationPage();
    void clearStation();
    void setupStationQML(QString stationType = "Laminar");
signals:
    void sendMessage(const QString msg, int timeout = 0);

public slots:
    void loadStation();
    void getProgress(QString progressName, int index, int total);
private:
    Ui::WidgetStationPage *ui;
};

#endif // WIDGETSTATIONPAGE_H
