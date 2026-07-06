#ifndef SOLITAIRE_H
#define SOLITAIRE_H

#include <vector>
#include "raylib.h"
#include "Card.h"
#include "Deck.h"
#include "Pile.h"
#include "Player.h"
#include "Game.h"
using namespace std;

struct Snapshot {
    vector<Card> stock;
    vector<Card> waste;
    vector<Card> foundations[4];
    vector<Card> tableau[7];
    int moves;
    bool won;
};

class Solitaire : public Game {
private:
    Deck deck;
    StockPile stock;
    WastePile waste;
    FoundationPile foundations[4];
    TableauPile tableau[7];
    HumanPlayer player;

    Pile* picked;
    int pickedindex;
    int moves;
    bool won;

    vector<Snapshot> history;

    Rectangle bnew;
    Rectangle bsave;
    Rectangle bload;
    Rectangle bundo;

    void placepiles();
    void newgame();
    void deal();

    void clickleft(Vector2 m);
    void drawstock();
    Pile* findcard(Vector2 m, int& index);
    Pile* finddest(Vector2 m);

    bool goodrun(Pile* p, int start);
    void trymove(Pile* dest);
    bool isfoundation(Pile* p);
    bool istableau(Pile* p);
    void checkwin();

    void savestate();
    void undo();

    void savegame();
    void loadgame();
    void loadstats();
    void savestats();

    void drawbutton(Rectangle r, const char* label);
    void drawpicked();

public:
    Solitaire();

    void start();
    void update();
    void draw();
    bool finished();
};

#endif