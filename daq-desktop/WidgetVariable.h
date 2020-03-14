#ifndef WIDGETVARIABLE_H
#define WIDGETVARIABLE_H

#include <QWidget>

#include <Variable/Variable.h>

namespace Ui {
class WidgetVariable;
}

class WidgetVariable : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetVariable(Variable* var, QWidget *parent = nullptr);
    ~WidgetVariable();

    int m_var_id;

private:
    Variable* mVariable;
    Ui::WidgetVariable *ui;
};

#endif // WIDGETVARIABLE_H
