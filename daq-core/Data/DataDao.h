#ifndef DATADAO_H
#define DATADAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Data;
class QMutex;

class DataDao
{
public:
    DataDao(QSqlDatabase& database, QMutex& dbMutex);
    void init() const;

    void addData(Data& data) const;
    void updateData(const Data& data) const;
    void removeData(int id) const;
    void removeAll() const;
    std::unique_ptr<std::vector<std::shared_ptr<Data>>> datas() const;
private:
    QSqlDatabase& mDatabase;
    QMutex& mMutex;
};

#endif // DATADAO_H
