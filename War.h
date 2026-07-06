#ifndef WAR_H
#define WAR_H
#include <vector>
#include "raylib.h"
#include "Game.h"
#include "Card.h"
#include "Player.h"
using namespace std;

//THE GAME RULES I USED: 
//2 players flip card one (more than 2 can play as well but i was a 2-player)
// at a time and the one with the higher rank wins so the loser 
// gets both cards added to the end of their pile, if both 
// players' crds have the same rank then it is a war and 5 
// more cards are placed each and the last card is flipped to 
// decide the winner. if the fifth cards are the same again then 
// it is war again and 5 more cards are placed each

class War : public Game {
private:
    vector<Card> playerhand;
    vector<Card> aihand;
    vector<Card> playerpile;
    vector<Card> aipile;
    HumanPlayer human;
    AIPlayer ai;
    Rectangle bnew;

    int phase;
    int roundresult;
    bool warthisbattle;
    bool playerwins;
    int cardvalue(Card c);
    void dealnewgame();
    void resolvebattle();
    void collecttowinner(bool toplayer);
    void checkend();
    void drawbutton(Rectangle r, const char* label);

public:
    War();
    void start();
    void update();
    void draw();
    bool finished();
};

#endif
