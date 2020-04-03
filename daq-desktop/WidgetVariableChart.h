#ifndef WIDGETVARIABLECHART_H
#define WIDGETVARIABLECHART_H

#include <QWidget>
#include <memory>
#include "Variable/Variable.h"
#include <QtCharts/QChartView>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <QValueAxis>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class WidgetVariableChart;
}

class WidgetVariableChart : public QWidget
{
    Q_OBJECT

public:
    WidgetVariableChart(const std::shared_ptr<Variable>& var, QWidget *parent = nullptr);
    ~WidgetVariableChart();
    void initChart(const std::shared_ptr<Variable>& var);
    void setVariable(const std::shared_ptr<Variable>& var);

public slots:
    void updateChart(QHash<QString,QVariant> singleData);

private:
    std::shared_ptr<Variable> mVar;
    QChartView mChartView;
    QLineSeries* mSeries;
    QDateTimeAxis *mAxisX;
    QValueAxis *mAxisY;
    Ui::WidgetVariableChart *ui;
    double dataMax;
    double dataMin;

};

#endif // WIDGETVARIABLECHART_H
