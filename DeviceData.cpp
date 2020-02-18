#include "DeviceData.h"




DeviceData::DeviceData()
{

}

DeviceData::DeviceData(int dataIndex,
                       QString name,
                       int cmdID,
                       DataPhysics ph,
                       FlowUnits flowUnit,
                       TemperatureUnits tempUnit,
                       PressureUnits pressUnit,
                       int paraIndex,
                       double dataMax,
                       double dataMin) :
                       m_dataIndex(dataIndex),
                       m_dataName(name),
                       m_linked_cmd_id(cmdID),
                       m_physics(ph),
                       m_flowUnit(flowUnit),
                       m_tempUnit(tempUnit),
                       m_pressUnit(pressUnit),
                       m_parameterIndex(paraIndex),
                       m_maxAllowed(dataMax),
                       m_minAllowed(dataMin)
{
    m_status = NotAvailable;
    m_currentData = 0.0;
}

void DeviceData::addData(double data, QDateTime curTime) {
    if (m_status == NotAvailable) m_status = Available;

    m_currentTimeStamp = curTime;

    m_prevData = m_currentData;
    m_currentData = data;

    if (m_historyData.size() >= MAX_HISTORY_DATA_SIZE){
        m_historyData.pop_back();
        m_historyData.push_front(data);

        m_historyDataTimeStamp.pop_back();
        m_historyDataTimeStamp.push_front(m_currentTimeStamp);

    } else {
        m_historyData.append(data);
        m_historyDataTimeStamp.append(m_currentTimeStamp);
    }

}
