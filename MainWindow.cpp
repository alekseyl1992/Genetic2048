#include <iostream>

#include <QtWidgets/QCheckBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Game2048.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    constexpr int w = Game2048::fieldWidth;
    constexpr int h = Game2048::fieldHeight;

    tableModel = new QStandardItemModel(h, w, this);

    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            QStandardItem *item = new QStandardItem("");
            tableModel->setItem(j, i, item);
        }
    }

    ui->fieldView->setModel(tableModel);
    ui->fieldView->resizeColumnsToContents();

    constexpr int chromosomesCount = 140;
    constexpr int colsCount = 1;
    statsModel = new QStandardItemModel(chromosomesCount, colsCount, this);

    for (int i = 0; i < chromosomesCount; ++i) {
        for (int j = 0; j < colsCount; ++j) {
            QStandardItem *item = new QStandardItem("0");
            statsModel->setItem(i, j, item);
        }
    }
    ui->statsView->setModel(statsModel);

    qRegisterMetaType<Game2048::Board>("Game2048::Board");
    qRegisterMetaType<Pool>("Pool");

    workerThread = new WorkerThread(chromosomesCount, this);
    workerThread->start();

    QObject::connect(workerThread,
                &WorkerThread::statsChanged,
                this,
                &MainWindow::renderStats);

    QObject::connect(ui->renderCheckBox,
                     &QCheckBox::stateChanged,
                     this,
                     &MainWindow::renderCheckboxStateChanged);

    // enable rendering
    QObject::connect(workerThread,
                    &WorkerThread::fieldChanged,
                    this,
                    &MainWindow::renderField);
}

void MainWindow::renderField(Game2048::Board board) {

    for(int j = 0; j < Game2048::fieldHeight; ++j) {
        for(int i = 0; i < Game2048::fieldWidth; ++i) {
            auto item = tableModel->item(j, i);

            auto val = board[i][j].val;
            if (val != 0)
                item->setText(QString::number(val));
            else
                item->setText("");
        }
    }

    ui->fieldView->repaint();
}

void MainWindow::renderStats(Pool pool) {
    for (size_t chromosomeId = 0; chromosomeId < pool.size(); ++chromosomeId) {
        Chromosome chromosome = pool[chromosomeId];

        auto fitnessItem = statsModel->item(chromosomeId, 0);
        fitnessItem->setText(QString::number(chromosome.fitness));
    }

    ui->statsView->repaint();
}

void MainWindow::renderCheckboxStateChanged(int state)
{
    if (state == Qt::CheckState::Checked) {
        QObject::connect(workerThread,
                        &WorkerThread::fieldChanged,
                        this,
                        &MainWindow::renderField);
    } else {
        QObject::disconnect(workerThread,
                        &WorkerThread::fieldChanged,
                        this,
                        &MainWindow::renderField);
    }
}

MainWindow::~MainWindow()
{
    workerThread->stop();
    workerThread->wait();
    delete ui;
}
