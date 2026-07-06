#include <iostream>
#include "Player.h"
using namespace std;

Player::Player(const char* n) {
    int i = 0;
    while (n[i] != '\0' && i < 31) {
        name[i] = n[i];
        i++;
    }
    name[i] = '\0';
    gamesplayed = 0;
    gameswon = 0;
}

Player::~Player() {
}

const char* Player::getname() {
    return name;
}

int Player::getplayed() {
    return gamesplayed;
}

int Player::getwon() {
    return gameswon;
}

void Player::setstats(int played, int wins) {
    gamesplayed = played;
    gameswon = wins;
}

void Player::addplayed() {
    gamesplayed++;
}

void Player::addwon() {
    gameswon++;
}

void Player::playturn() {
    cout << "a player takes a turn" << endl;
}

HumanPlayer::HumanPlayer(const char* n) : Player(n) {
}

void HumanPlayer::playturn() {
    cout << "your turn" << endl;
}

AIPlayer::AIPlayer(const char* n) : Player(n) {
}

void AIPlayer::playturn() {
    cout << "computer's turn" << endl;
}
