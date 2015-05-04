#include "WorkerThread.h"
#include "Genetic.h"
#include <iostream>

WorkerThread::WorkerThread(int populationSize, QObject *parent) :
    QThread(parent), populationSize(populationSize)
{
}

void WorkerThread::run()
{
    Genetic genetic(populationSize, 1./2000.);
    genetic.init();

    loop = true;
    while(loop) {
        int result = gameRun(genetic);
        //emit statsChanged(genetic.getPopulation());

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

    auto result = game.loop();

//    if (result.reason == GameResult::WRONG_MOVE)
//        genetic.mutateCurrent();

    return result.score;
}
