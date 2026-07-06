#include <iostream>
#include "setup.h"
#include "Solitaire.h"
#include "War.h"
#include "Match.h"
#include <iomanip>
using namespace std;

int main() {
    int choice = 1;

    while (choice != 0) {
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << setw(200) << "================================" << endl;
        cout << setw(200) << "OOP FINAL PROJECT - 3 CARD GAMES" << endl;
        cout << setw(200) << "================================" << endl;
        cout << setw(200) << "1 - Solitaire                   " << endl;
        cout << setw(200) << "2 - WAR                         " << endl;
        cout << setw(200) << "3 - Memory match the cards      " << endl;
        cout << setw(200) << "0 - EXIT                        " << endl;
        cout << setw(200) << "================================" << endl;
        cout << setw(200) << "Enter your choice:              ";
        cin >> choice;

        if (choice == 2) {
            War game;
            game.run(windoww, windowh, "War");
        }
        else if (choice == 3) {
            Match game;
            game.run(windoww, windowh, "Match");
        }
        else {
            Solitaire game;
            game.run(windoww, windowh, "Solitaire");
        }
    }
    return 0;
}
