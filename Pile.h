#ifndef PILE_H
#define PILE_H
#include <vector>
#include "Card.h"
using namespace std;

///all the pile classes together because they were all too small to need seperate files each
class Pile {
protected:
    vector<Card> cards;
    int x;
    int y;



public:
    Pile();
    virtual ~Pile();
    void setpos(int px, int py);
    int getx();
    int gety();
    void addcard(Card c);
    Card removetop();
    Card topcard();
    Card& cardat(int i);
    bool isempty();
    int count();
    void clear();
    virtual bool canplace(Card c);
    virtual void draw();

};

class TableauPile : public Pile {
public:
    bool canplace(Card c);
    void draw();
};

class FoundationPile : public Pile {
public:
    bool canplace(Card c);
};



class WastePile : public Pile {
};



class StockPile : public Pile {
};

#endif
