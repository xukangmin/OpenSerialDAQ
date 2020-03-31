#include "WidgetVariable.h"
#include "ui_WidgetVariable.h"

#include <QDateTimeAxis>
#include <QLineSeries>
#include <QValueAxis>


WidgetVariable::WidgetVariable(Variable* var, QWidget *parent) :
    QWidget(parent), m_var_id(var->m_id), mVariable(var),
    ui(new Ui::WidgetVariable)
{
    ui->setupUi(this);

    ui->verticalLayout_2->setAlignment(Qt::AlignTop);

    ui->leName->setText(var->getSingleProperty("Name").toString());

    ui->leUnit->setText(var->getSingleProperty("Unit").toString());

    ui->leCurrentValue->setText(var->getSingleProperty("CurrentValue").toString());

    if (var->historyData.size() > 0) {
        QLineSeries* series = new QLineSeries();
        foreach(auto& prop, var->historyData)
        {
            QHash<QString,QVariant> singleData = qvariant_cast<QHash<QString,QVariant>>(prop);

            series->append(singleData["TimeStamp"].toDateTime().toMSecsSinceEpoch(), singleData["RealValue"].toDouble());
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->legend()->hide();
        chart->setTitle("Sunspots count (by Space Weather Prediction Center)");
        //![3]

        //![4]
        QDateTimeAxis *axisX = new QDateTimeAxis;
        axisX->setTickCount(10);
        axisX->setFormat("hh:mm:ss.zzz");
        axisX->setTitleText("Time");
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis;
        axisY->setLabelFormat("%i");
        axisY->setTitleText(var->getSingleProperty("Name").toString());
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
        //![4]

        //![5]
        mChartView = new QChartView(chart);

        ui->verticalLayout_2->addWidget(mChartView);
    }
}

WidgetVariable::~WidgetVariable()
{
    delete ui;
}
