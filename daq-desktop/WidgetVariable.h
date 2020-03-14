#ifndef WIDGETVARIABLE_H
#define WIDGETVARIABLE_H

#include <QWidget>

namespace Ui {
class WidgetVariable;
}

class WidgetVariable : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetVariable(QWidget *parent = nullptr);
    ~WidgetVariable();

private:
    Ui::WidgetVariable *ui;
};

#endif // WIDGETVARIABLE_H
