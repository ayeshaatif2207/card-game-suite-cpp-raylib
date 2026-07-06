#ifndef CARD_H
#define CARD_H
#include "raylib.h"

enum Suit { hearts, diamonds, clubs, spades };

class Card {
private:
    Suit suit;
    int rank;
    bool faceup;

public:
    Card();
    Card(Suit s, int r);
    void flip();
    void setface(bool up);
    bool isfaceup();
    Suit getsuit();
    int getrank();
    bool isred();
    const char* ranktext();
    char suitletter();



    void draw(int x, int y);




};

#endif
