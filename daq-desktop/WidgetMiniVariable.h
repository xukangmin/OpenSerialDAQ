#ifndef WIDGETMINIVARIABLE_H
#define WIDGETMINIVARIABLE_H

#include <QWidget>

#include <Variable/Variable.h>

namespace Ui {
class WidgetMiniVariable;
}

class WidgetMiniVariable : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMiniVariable(Variable* var, QWidget *parent = nullptr);
    ~WidgetMiniVariable();

private:
    Ui::WidgetMiniVariable *ui;
    Variable *mVar;
};

#endif // WIDGETMINIVARIABLE_H
