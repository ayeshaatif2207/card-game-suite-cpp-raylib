#include "raylib.h"
#include "Pile.h"
#include "setup.h"

Pile::Pile() {
    x = 0;
    y = 0;
}

Pile::~Pile() {
}

void Pile::setpos(int px, int py) {
    x = px;
    y = py;
}

int Pile::getx() {
    return x;
}

int Pile::gety() {
    return y;
}

void Pile::addcard(Card c) {
    cards.push_back(c);
}

Card Pile::removetop() {
    Card c = cards[cards.size() - 1];
    cards.pop_back();
    return c;
}

Card Pile::topcard() {
    return cards[cards.size() - 1];
}

Card& Pile::cardat(int i) {
    return cards[i];
}

bool Pile::isempty() {
    return cards.size() == 0;
}

int Pile::count() {
    return (int)cards.size();
}

void Pile::clear() {
    cards.clear();
}

bool Pile::canplace(Card c) {
    return false;
}

void Pile::draw() {
    if (cards.size() == 0) {
        DrawRectangleLines(x, y, cardw, cardh, DARKGRAY);
        return;
    }
    cards[cards.size() - 1].draw(x, y);
}

bool TableauPile::canplace(Card c) {
    if (cards.size() == 0) {
        return c.getrank() == 13;
    }
    Card top = cards[cards.size() - 1];
    bool oppositecolour = (top.isred() != c.isred());
    bool onelower = (c.getrank() == top.getrank() - 1);
    return oppositecolour && onelower;
}

void TableauPile::draw() {
    if (cards.size() == 0) {
        DrawRectangleLines(x, y, cardw, cardh, DARKGRAY);
        return;
    }
    int i;
    for (i = 0; i < (int)cards.size(); i++) {
        cards[i].draw(x, y + i * fany);
    }
}

bool FoundationPile::canplace(Card c) {
    if (cards.size() == 0) {
        return c.getrank() == 1;
    }
    Card top = cards[cards.size() - 1];
    bool samesuit = (top.getsuit() == c.getsuit());
    bool onehigher = (c.getrank() == top.getrank() + 1);
    return samesuit && onehigher;
}
