#ifndef WIDGETSETTINGPAGE_H
#define WIDGETSETTINGPAGE_H

#include <QWidget>

namespace Ui {
class WidgetSettingPage;
}

class WidgetSettingPage : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetSettingPage(QWidget *parent = nullptr);
    ~WidgetSettingPage();

private:
    Ui::WidgetSettingPage *ui;
};

#endif // WIDGETSETTINGPAGE_H
