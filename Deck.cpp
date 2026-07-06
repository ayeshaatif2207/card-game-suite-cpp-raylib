#include "raylib.h"
#include "Deck.h"




Deck::Deck() {
}


void Deck::build() {
    cards.clear();
    int s;
    int r;
    for (s = 0; s < 4; s++) {
        for (r = 1; r <= 13; r++) {
            Card c((Suit)s, r);
            cards.push_back(c);
        }
    }
}

void Deck::shuffle() {
    int i;
    for (i = (int)cards.size() - 1; i > 0; i--) {
        int j = GetRandomValue(0, i);
        Card temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}

Card Deck::drawcard() {
    Card c = cards[cards.size() - 1];
    cards.pop_back();
    return c;
}

bool Deck::isempty() {
    return cards.size() == 0;
}

int Deck::count() {
    return (int)cards.size();
}
