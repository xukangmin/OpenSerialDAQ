#ifndef CHANNELDAO_H
#define CHANNELDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Channel;
class ThreadChannel;

class ChannelDao
{
public:
    ChannelDao(QSqlDatabase& database);
    void init() const;

    void addChannel(Channel& ch) const;
    void updateChannel(const Channel& ch) const;
    void removeChannel(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<Channel>>> channels() const;
    std::unique_ptr<std::vector<std::unique_ptr<ThreadChannel>>> threadChannels() const;
private:
    QSqlDatabase& mDatabase;
};

#endif // CHANNELDAO_H
