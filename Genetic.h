#ifndef TETRIS_GENETIC_H
#define TETRIS_GENETIC_H
#include <vector>
#include <boost/circular_buffer.hpp>
#include "Game2048.h"
#include "Chromosome.h"

using GeneticRow = std::vector<int>;
using GeneticField = std::vector<GeneticRow>;
using Pool = std::vector<Chromosome>;

class Genetic {
public:
    Genetic(int populationSize = 20, double mutationProbability = 1./50.);

    movDir activate(const Game2048::Board& board);
    void init();
    void step(int score);

    void newGeneration();
    void newGeneration2();

    GeneticField createGeneticField(const Game2048::Board& board);
    void printGeneticField(GeneticField &geneticField);
    std::vector<double> geneticFieldToInput(const GeneticField& field);

    int getCurrentChromosomeId() const;
    Pool getPopulation() const;

    void mutateCurrent();

private:
    int populationSize;
    std::vector<int> nnSizes;
    size_t chromosomeSize;
    double mutationProbability;
    Pool pool;
    int currentChromosomeId;

    static constexpr int historySize = 10;
    boost::circular_buffer<double> buttonsHistory;

    int maxScores = 0;
};

#endif //TETRIS_GENETIC_H
