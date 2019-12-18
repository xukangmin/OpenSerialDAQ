#ifndef SINGLECHANNEL_H
#define SINGLECHANNEL_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QTimer>

class SingleChannel : public QThread
{
    Q_OBJECT

public:
    explicit SingleChannel(QObject *parent = nullptr);
    ~SingleChannel();

    void startChannel();

    void stopChannel();

private:
    void run() override;
    QTimer m_timer;
    QMutex m_mutex;
};

#endif // SINGLECHANNEL_H
