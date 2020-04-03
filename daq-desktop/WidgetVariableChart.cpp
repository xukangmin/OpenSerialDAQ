#include "WidgetVariableChart.h"
#include "ui_WidgetVariableChart.h"

#include <QVBoxLayout>



WidgetVariableChart::WidgetVariableChart(const std::shared_ptr<Variable>& var, QWidget *parent) :
    QWidget(parent), mVar(var), mChartView(this), mSeries(new QLineSeries(this)), mAxisX(new QDateTimeAxis(this)), mAxisY(new QValueAxis(this)),
    ui(new Ui::WidgetVariableChart)
{
    ui->setupUi(this);

    connect(var.get(), &Variable::sendDataToChart, this, &WidgetVariableChart::updateChart);

    mChartView.setRenderHint(QPainter::Antialiasing);
    mChartView.chart()->legend()->setVisible(false);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(&mChartView);
    setLayout(layout);

    initChart(var);


}

WidgetVariableChart::~WidgetVariableChart()
{
    delete ui;
}

void WidgetVariableChart::setVariable(const std::shared_ptr<Variable>& var)
{
    disconnect(mVar.get(), &Variable::sendDataToChart, this, &WidgetVariableChart::updateChart);
    connect(var.get(), &Variable::sendDataToChart, this, &WidgetVariableChart::updateChart);

    mVar = var;

    mSeries->clear();

    initChart(var);
}

void WidgetVariableChart::initChart(const std::shared_ptr<Variable>& var)
{
    QHash<QString,QVariant> firstData = qvariant_cast<QHash<QString,QVariant>>(var->historyData.at(0));
    dataMax = firstData["RealValue"].toDouble();
    dataMin = dataMax;

    foreach(auto& prop, var->historyData)
    {
        QHash<QString,QVariant> singleData = qvariant_cast<QHash<QString,QVariant>>(prop);

        mSeries->append(singleData["TimeStamp"].toDateTime().toMSecsSinceEpoch(), singleData["RealValue"].toDouble());


        if (singleData["RealValue"].toDouble() > dataMax) {
            dataMax = singleData["RealValue"].toDouble();
        }

        if (singleData["RealValue"].toDouble() < dataMin) {
            dataMin = singleData["RealValue"].toDouble();
        }
    }

    QChart *mChart = mChartView.chart();

    mChart->addSeries(mSeries);
    mChart->legend()->hide();
    mChart->setTitle("Data: " + var->getSingleProperty("Name").toString());

     mAxisX->setTickCount(10);
     mAxisX->setFormat("hh:mm:ss");
     mAxisX->setTitleText("Time");
     mChart->addAxis(mAxisX, Qt::AlignBottom);
     mSeries->attachAxis(mAxisX);

     mAxisY->setLabelFormat("%f");
     mAxisY->setTitleText(mVar->getSingleProperty("Name").toString());
     mChart->addAxis(mAxisY, Qt::AlignLeft);
     mSeries->attachAxis(mAxisY);

}

void WidgetVariableChart::updateChart(QHash<QString,QVariant> singleData)
{

//    foreach(auto& prop, mVar->historyData)
//    {
//        QHash<QString,QVariant> singleData = qvariant_cast<QHash<QString,QVariant>>(prop);

//        mSeries->append(singleData["TimeStamp"].toDateTime().toMSecsSinceEpoch(), singleData["RealValue"].toDouble());
//    }
     mSeries->append(singleData["TimeStamp"].toDateTime().toMSecsSinceEpoch(), singleData["RealValue"].toDouble());

     // to do, overflow handling

     mAxisX->setMax(singleData["TimeStamp"].toDateTime());

     if (singleData["RealValue"].toDouble() > dataMax) {
         dataMax = singleData["RealValue"].toDouble();
     }

     if (singleData["RealValue"].toDouble() < dataMin) {
         dataMin = singleData["RealValue"].toDouble();
     }

     double margin = 0.2 * (dataMax - dataMin);

    mAxisY->setRange(dataMin - margin , dataMax + margin);

    //mChart->update();
}
