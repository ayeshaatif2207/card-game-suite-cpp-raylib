#ifndef MATCH_H
#define MATCH_H
#include <vector>
#include "raylib.h"
#include "Game.h"
#include "Card.h"
#include "Player.h"
using namespace std;

class Match : public Game {
private:
    vector<Card> board;
    vector<bool> matched;
    int rows;
    int cols;
    int firstpick;
    int secondpick;
    bool previewing;
    double previewstart;
    int attempts;
    int pairsfound;
    bool won;
    HumanPlayer human;
    Rectangle bnew;



    void buildboard();
    void cellposition(int index, int& outx, int& outy);
    int cardatposition(Vector2 m);
    void drawbutton(Rectangle r, const char* label);

public:
    Match();
    void start();
    void update();
    void draw();
    bool finished();
};

#endif
