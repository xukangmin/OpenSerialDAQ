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

private:
    Ui::WidgetStationPage *ui;
    VariableProxyModel* mProxyModel;
};

#endif // WIDGETSTATIONPAGE_H
