#ifndef CHANNELDAO_H
#define CHANNELDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Channel;
class ThreadChannel;
class QMutex;

class ChannelDao
{
public:
    ChannelDao(QSqlDatabase& database, QMutex &mutex);
    void init() const;

    void addChannel(Channel& ch) const;
    void updateChannel(const Channel& ch) const;
    void removeChannel(int id) const;
    void removeAll() const;
    std::unique_ptr<std::vector<std::unique_ptr<Channel>>> channels() const;
private:
    QSqlDatabase& mDatabase;
    QMutex& mMutex;
};

#endif // CHANNELDAO_H
