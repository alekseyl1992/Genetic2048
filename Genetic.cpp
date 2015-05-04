#include <iostream>
#include <cmath>
#include <memory>
#include <array>
#include <algorithm>
#include "Game2048.h"
#include "Genetic.h"
#include "util.h"


Genetic::Genetic(int populationSize, double mutationProbability)
    : populationSize(populationSize),
      buttonsHistory(historySize * movDir::MOV_COUNT),
      nnSizes({Game2048::fieldWidth * Game2048::fieldHeight
              + historySize * movDir::MOV_COUNT,
              10, 10, MOV_COUNT}),
      chromosomeSize(0),
      mutationProbability(mutationProbability),
      currentChromosomeId(-1) {

    assert(mutationProbability < 1);

    for (size_t i = 1; i < nnSizes.size(); ++i) {
        auto prevLayerSize = nnSizes[i - 1];
        auto layerSize = nnSizes[i];
        chromosomeSize += (prevLayerSize + 1) * layerSize;
    }

    buttonsHistory.resize(historySize * movDir::MOV_COUNT, 0);
}

void Genetic::init() {
    for (int i = 0; i < populationSize; ++i) {
        Chromosome chromosome = Chromosome::createRandom(chromosomeSize);
        pool.push_back(chromosome);
    }
    currentChromosomeId = 0;
}

movDir Genetic::activate(const Game2048::Board& board) {
    GeneticField geneticField = createGeneticField(board);
//    printGeneticField(geneticField);

    // chromosome = [ l1_11 l1_21 .. l1_j1 l1_b1 ... l1_1s l1_2s .. l1_js l1_bs .....  ]

    typedef std::vector<double> vec;
    typedef std::shared_ptr<vec> vec_ptr;

    vec input = geneticFieldToInput(geneticField);
    // append buttons history to input vector
    for (auto button: buttonsHistory) {
        input.push_back(button);
    }

    vec_ptr layer_p = nullptr;
    vec_ptr prevLayer_p = std::make_shared<vec>(input);

    int c = 0;
    for (size_t layerId = 1; layerId < nnSizes.size(); ++layerId) {
        int layerSize = nnSizes[layerId];
        int prevLayerSize = nnSizes[layerId - 1];

        layer_p = std::make_shared<vec>(layerSize);
        vec& layer = *layer_p;
        vec& prevLayer = *prevLayer_p;
        for (int i = 0; i < layerSize; ++i) {
            for (int j = 0; j < prevLayerSize; ++j) {
                layer[i] += pool[currentChromosomeId].genome[c] * prevLayer[j];
                c += 1;
            }
            layer[i] += pool[currentChromosomeId].genome[c++];  // bias
            layer[i] = std::atan(layer[i]);  // activate neuron
        }

        prevLayer_p = layer_p;

    }

    auto result = std::max_element(layer_p->begin(), layer_p->end());
    int buttonId = std::distance(layer_p->begin(), result);

    std::array<double, movDir::MOV_COUNT> historyEntry;
    historyEntry.fill(0);
    historyEntry[buttonId] = 1;
    for (auto button: historyEntry) {
        buttonsHistory.push_back(button);
    }

    return (movDir) buttonId;
}

void Genetic::printGeneticField(GeneticField &geneticField) {
    std::cout << "=================================" << std::endl;
    for (size_t i = 0; i < geneticField.size(); ++i) {
        for (size_t j = 0; j < geneticField[i].size(); ++j) {
            std::cout << geneticField[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Genetic::step(int score) {
    pool[currentChromosomeId].fitness = score;

    if (++currentChromosomeId >= pool.size()) {  // end of generation
        newGeneration();
        currentChromosomeId = 0;
    }
}

GeneticField Genetic::createGeneticField(const Game2048::Board& board) {
    // project field
    GeneticField geneticField(Game2048::fieldHeight);
    for (int i = 0; i < Game2048::fieldHeight; ++i) {
        GeneticRow geneticRow(Game2048::fieldWidth);
        for (int j = 0; j < Game2048::fieldWidth; ++j) {
            int value = board[j][i].val;
            geneticRow[j] = value;
        }

        geneticField[i] = geneticRow;
    }

    //printGeneticField(geneticField);

    return geneticField;
}

std::vector<double> Genetic::geneticFieldToInput(const GeneticField& field) {
    std::vector<double> input(field.size() * field[0].size());
    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[i].size(); ++j) {
            input[i * field[i].size() + j] = static_cast<double>(field[i][j]);
        }
    }
    return input;
}

int Genetic::getCurrentChromosomeId() const
{
    return currentChromosomeId;
}

Pool Genetic::getPopulation() const
{
    return pool;
}

void Genetic::mutateCurrent()
{
    pool[currentChromosomeId].mutate(mutationProbability);
}

void Genetic::newGeneration() {
    std::sort(pool.begin(), pool.end(), [](auto& lhs, auto& rhs) {
        return lhs.fitness > rhs.fitness;
    });
    
    for (size_t chromosomeId = 0; chromosomeId < pool.size(); ++chromosomeId) {
        Chromosome chromosome = pool[chromosomeId];

        std::cout << chromosome.fitness << "\t";
    }
    std::cout << std::endl;

    size_t middle = pool.size() / 2;

    // are we at local maximum?
    int maxClassLen = 0;
    int maxClassBegin = 0;

    int curClassLen = 1;
    int curClassBegin = 0;

    int curFitness = pool[0].fitness;
    for (int i = 1; i < middle; ++i) {
        if (pool[i].fitness == curFitness) {
            ++curClassLen;
        } else {
            if (curClassLen > maxClassLen) {
                maxClassLen = curClassLen;
                maxClassBegin = curClassBegin;
            }

            curClassLen = 1;
            curClassBegin = i;
            curFitness = pool[i].fitness;
        }
    }

    if (maxClassLen > middle / 5) {
        // kill that class
        for (size_t i = maxClassBegin; i < maxClassBegin + maxClassLen; ++i) {
            pool[i] = Chromosome::createRandom(chromosomeSize);
        }
    }

    for (size_t i = middle; i < pool.size(); ++i) {
        auto& c1 = pool[randAB(0, middle)];
        auto& c2 = pool[randAB(middle, pool.size())];

        pool[i] = Chromosome::crossover(c1, c2);
        pool[i].mutate(mutationProbability);
    }
}

void Genetic::newGeneration2() {
    std::sort(pool.begin(), pool.end(), [](auto& lhs, auto& rhs) {
        return lhs.fitness > rhs.fitness;
    });

    size_t middle = pool.size() / 2;
    size_t j = 0;
    for (size_t i = middle; i < pool.size(); ++i) {
        auto& c1 = pool[j];
        auto c1Clone = c1.clone();
        pool[i] = c1Clone.mutateShuffle(0.3);
        if (rand01() < 0.08) {
            pool[j].mutateShuffle(0.3);
        }
        ++j;
    }
}
