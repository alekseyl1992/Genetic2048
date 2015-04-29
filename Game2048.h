#ifndef GAME2048_H
#define GAME2048_H

#include <QObject>

typedef unsigned int uint;
using namespace std;
enum movDir { UP, DOWN, LEFT, RIGHT, MOV_COUNT };

class Genetic;

class tile
{
public:
    tile() : val( 0 ), blocked( false ) {}
    uint val;
    bool blocked;
};

using Board = tile[4][4];

class Game2048 : public QObject
{
    Q_OBJECT
public:
    static constexpr int fieldWidth = 4;
    static constexpr int fieldHeight = 4;

    Game2048(Genetic& genetic, QObject *parent = 0);
    ~Game2048();

    int loop();

private:
    Board board;
    bool win, done, moved;
    uint score;

    void drawBoard();
    void waitKey();
    void addTile();
    bool canMove();
    bool testAdd( int x, int y, uint v );
    void moveVert( int x, int y, int d );
    void moveHori( int x, int y, int d );
    void move( movDir d );

signals:
    void render(Board board);

public slots:
};

#endif // GAME2048_H
