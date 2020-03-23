#ifndef DEVICEDAO_H
#define DEVICEDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Device;
class QMutex;

class DeviceDao
{
public:
    DeviceDao(QSqlDatabase& database, QMutex& dbMutex);
    void init() const;

    void addDevice(Device& ch) const;
    void updateDevice(const Device& ch) const;
    void removeDevice(int id) const;
    std::unique_ptr<std::vector<std::shared_ptr<Device>>> devices() const;
private:
    QSqlDatabase& mDatabase;
    QMutex& mMutex;
};

#endif // DEVICEDAO_H
