#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include "Unit/Unit.h"
#include <QDateTime>
#include <QVector>
#include <QMetaType>
#include <limits>



class Variable : public QObject
{
    Q_OBJECT
public:
    explicit Variable(QObject *parent = nullptr);

    Variable(int id, QString name, QString unit, QString equation = "N/A");

    int m_id;  // variable id

    QString m_name;

    int m_device_id;

    QString m_unitStr;

    Unit m_unit;

    QString m_equation;

    QVector<int> source_variable_list;

    void addData(double data, QDateTime curTime = QDateTime::currentDateTime());

    int m_dataIndex;

    QString m_dataName;

    int m_linked_cmd_id;

    int m_parameterIndex;



    double m_maxAllowed;

    double m_minAllowed;

    QByteArray m_extraQueryData;

    int m_dataTypeID;

    QDateTime m_currentTimeStamp;

    double m_currentData;

    double m_prevData;

    double m_maxData;

    double m_minData;

    QVector<double> m_historyData;

    QVector<QDateTime> m_historyDataTimeStamp;


signals:

};

#endif // VARIABLE_H
