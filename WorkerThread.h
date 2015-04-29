#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include "Game2048.h"
#include "Genetic.h"

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(int populationSize, QObject *parent=nullptr);
    virtual void stop();

private:
    int gameRun(Genetic& genetic);
    bool loop;
    int populationSize;

protected:
    virtual void run();

signals:
    void fieldChanged(Game2048::Board board);
    void statsChanged(const Pool& pool);

public slots:

};

#endif // WORKERTHREAD_H
