#ifndef COMMANDWIDGET_H
#define COMMANDWIDGET_H

#include <QGroupBox>

namespace Ui {
class CommandWidget;
}

class CommandWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit CommandWidget(QWidget *parent = nullptr);
    ~CommandWidget();

private:
    Ui::CommandWidget *ui;
};

#endif // COMMANDWIDGET_H
