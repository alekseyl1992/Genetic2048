#include "WorkerThread.h"
#include "Genetic.h"
#include <iostream>

WorkerThread::WorkerThread(int populationSize, QObject *parent) :
    QThread(parent), populationSize(populationSize)
{
}

void WorkerThread::run()
{
    Genetic genetic(populationSize, 1./100.);
    genetic.init();

    loop = true;
    while(loop) {
        int result = gameRun(genetic);
        //emit statsChanged(genetic.getPopulation());

        Pool pool = genetic.getPopulation();

        for (size_t chromosomeId = 0; chromosomeId < pool.size(); ++chromosomeId) {
            Chromosome chromosome = pool[chromosomeId];

            std::cout << chromosome.fitness << "\t";
        }
        std::cout << std::endl;

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
