#ifndef WIDGETMINIVARIABLE_H
#define WIDGETMINIVARIABLE_H

#include <QWidget>

namespace Ui {
class WidgetMiniVariable;
}

class WidgetMiniVariable : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMiniVariable(QWidget *parent = nullptr);
    ~WidgetMiniVariable();

private:
    Ui::WidgetMiniVariable *ui;
};

#endif // WIDGETMINIVARIABLE_H
