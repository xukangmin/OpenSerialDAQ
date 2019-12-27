#ifndef SINGLECHANNEL_H
#define SINGLECHANNEL_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QTimer>
#include <QQueue>

class SingleChannel : public QThread
{
    Q_OBJECT

public:
    explicit SingleChannel(QObject *parent = nullptr);
    ~SingleChannel() override;

    void startChannel();

    void stopChannel();

private:
    void run() override;
    QTimer* m_timer1;
    QTimer* m_timer2;
    QMutex m_mutex;
    QQueue<QString> m_dataQueue;

private slots:
    void trigger1();
    void trigger2();
};

#endif // SINGLECHANNEL_H
