#include "Game2048.h"
#include "Genetic.h"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

Game2048::Game2048(Genetic& genetic, QObject *parent)
    : genetic(genetic), QObject(parent),
      done( false ), win( false ), moved( true ), score( 0 )
{
    srand(4564523);
}

Game2048::~Game2048()
{ }

int Game2048::loop()
{
    addTile();
    while( true )
    {
        if (moved)
            addTile();

        drawBoard();

        if (done)
            break;

        waitKey();

        if (!moved) //nn makes wrong move
            return score;
    }
//    string s = "Game Over!";
//    if( win ) s = "You've made it!";
//    std::cout << s << endl << endl;

    if( win )
        std::cout << "Wow!" << std::endl;

    return score;
}

void Game2048::drawBoard()
{
    //emit render(board);
}

void Game2048::waitKey()
{
    moved = false;

    auto button = genetic.activate(board);
    move(button);

    for( int y = 0; y < 4; y++ )
        for( int x = 0; x < 4; x++ )
            board[x][y].blocked = false;
}

void Game2048::addTile()
{
    for( int y = 0; y < 4; y++ )
        for( int x = 0; x < 4; x++ )
            if( !board[x][y].val )
            {
                uint a, b;
                do
                { a = rand() % 4; b = rand() % 4; }
                while( board[a][b].val );

                int s = rand() % 100;
                if( s > 89 ) board[a][b].val = 4;
                else board[a][b].val = 2;
                if( canMove() ) return;
            }
    done = true;
}

bool Game2048::canMove()
{
    for( int y = 0; y < 4; y++ )
        for( int x = 0; x < 4; x++ )
            if( !board[x][y].val ) return true;

    for( int y = 0; y < 4; y++ )
        for( int x = 0; x < 4; x++ )
        {
            if( testAdd( x + 1, y, board[x][y].val ) ) return true;
            if( testAdd( x - 1, y, board[x][y].val ) ) return true;
            if( testAdd( x, y + 1, board[x][y].val ) ) return true;
            if( testAdd( x, y - 1, board[x][y].val ) ) return true;
        }
    return false;
}

bool Game2048::testAdd(int x, int y, uint v)
{
    if( x < 0 || x > 3 || y < 0 || y > 3 ) return false;
    return board[x][y].val == v;
}

void Game2048::moveVert(int x, int y, int d)
{
    if( board[x][y + d].val && board[x][y + d].val == board[x][y].val && !board[x][y].blocked && !board[x][y + d].blocked  )
    {
        board[x][y].val = 0;
        board[x][y + d].val *= 2;
        score += board[x][y + d].val;
        board[x][y + d].blocked = true;
        moved = true;
    }
    else if( !board[x][y + d].val && board[x][y].val )
    {
        board[x][y + d].val = board[x][y].val;
        board[x][y].val = 0;
        moved = true;
    }
    if( d > 0 ) { if( y + d < 3 ) moveVert( x, y + d,  1 ); }
    else        { if( y + d > 0 ) moveVert( x, y + d, -1 ); }
}

void Game2048::moveHori(int x, int y, int d)
{
    if( board[x + d][y].val && board[x + d][y].val == board[x][y].val && !board[x][y].blocked && !board[x + d][y].blocked  )
    {
        board[x][y].val = 0;
        board[x + d][y].val *= 2;
        score += board[x + d][y].val;
        board[x + d][y].blocked = true;
        moved = true;
    }
    else if( !board[x + d][y].val && board[x][y].val )
    {
        board[x + d][y].val = board[x][y].val;
        board[x][y].val = 0;
        moved = true;
    }
    if( d > 0 ) { if( x + d < 3 ) moveHori( x + d, y,  1 ); }
    else        { if( x + d > 0 ) moveHori( x + d, y, -1 ); }
}

void Game2048::move(movDir d)
{
    //using namespace std::literals;
    //std::this_thread::sleep_for(1s);

    switch( d )
    {
    case UP:
        for( int x = 0; x < 4; x++ )
        {
            int y = 1;
            while( y < 4 )
            { if( board[x][y].val ) moveVert( x, y, -1 ); y++;}
        }
        break;
    case DOWN:
        for( int x = 0; x < 4; x++ )
        {
            int y = 2;
            while( y >= 0 )
            { if( board[x][y].val ) moveVert( x, y, 1 ); y--;}
        }
        break;
    case LEFT:
        for( int y = 0; y < 4; y++ )
        {
            int x = 1;
            while( x < 4 )
            { if( board[x][y].val ) moveHori( x, y, -1 ); x++;}
        }
        break;
    case RIGHT:
        for( int y = 0; y < 4; y++ )
        {
            int x = 2;
            while( x >= 0 )
            { if( board[x][y].val ) moveHori( x, y, 1 ); x--;}
        }
    }
}

