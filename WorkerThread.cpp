#include "WorkerThread.h"
#include "Genetic.h"

WorkerThread::WorkerThread(int populationSize, QObject *parent) :
    QThread(parent), populationSize(populationSize)
{
}

void WorkerThread::run()
{
    Genetic genetic(populationSize, 1./10.);
    genetic.init();

    loop = true;
    while(loop) {
        int result = gameRun(genetic);
        emit statsChanged(genetic.getPopulation());

        genetic.step(result);
    }
}

void WorkerThread::stop()
{
    loop = false;
}

int WorkerThread::gameRun(Genetic& genetic) {
    Game2048 game(genetic);
    QObject::connect(&game,
                     &Game2048::render,
                     [&] (const Game2048::Board& board) {
        emit fieldChanged(board);
    });

    auto score = game.loop();

    return score;
}
