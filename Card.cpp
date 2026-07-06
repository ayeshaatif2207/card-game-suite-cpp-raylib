#include "raylib.h"
#include "Card.h"
#include "setup.h"





Card::Card() {
    suit = hearts;
    rank = 1;
    faceup = false;
}

Card::Card(Suit s, int r) {
    suit = s;
    rank = r;
    faceup = false;
}

void Card::flip() {
    faceup = !faceup;
}

void Card::setface(bool up) {
    faceup = up;
}

bool Card::isfaceup() {
    return faceup;
}

Suit Card::getsuit() {
    return suit;
}

int Card::getrank() {
    return rank;
}

bool Card::isred() {
    return suit == hearts || suit == diamonds;
}

const char* Card::ranktext() {
    if (rank == 1) return "A";
    if (rank == 2) return "2";
    if (rank == 3) return "3";
    if (rank == 4) return "4";
    if (rank == 5) return "5";
    if (rank == 6) return "6";
    if (rank == 7) return "7";
    if (rank == 8) return "8";
    if (rank == 9) return "9";
    if (rank == 10) return "10";
    if (rank == 11) return "J";
    if (rank == 12) return "Q";
    if (rank == 13) return "K";
    return "-";
}

char Card::suitletter() {
    if (suit == hearts) return 'H';
    if (suit == diamonds) return 'D';
    if (suit == clubs) return 'C';
    return 'S';
}

void Card::draw(int x, int y) {
    if (faceup) {
        DrawRectangle(x, y, cardw, cardh, RAYWHITE);
        DrawRectangleLines(x, y, cardw, cardh, BLACK);
        Color c = BLACK;
        if (isred()) c = RED;
        DrawText(ranktext(), x + 6, y + 6, 24, c);
        char letter[2];
        letter[0] = suitletter();
        letter[1] = '\0';
        DrawText(letter, x + 6, y + 34, 24, c);
    }
    else {
        DrawRectangle(x, y, cardw, cardh, DARKBLUE);
        DrawRectangleLines(x, y, cardw, cardh, BLACK);
    }
}
