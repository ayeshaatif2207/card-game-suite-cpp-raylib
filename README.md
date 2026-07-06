# card-game-suite-cpp-raylib
I built a card game suite in C++ using raylib for my OOP final semester project, with three fully playable games: -Klondike Solitaire: drag-and-drop, undo, save/load, and move tracking -War: against an AI opponent, with the full "war" tiebreak mechanic -Memory Match: flip and match the pairs 

Everything sits on one abstract Game base class. Each game inherits from it and implements four methods that's it. Adding a brand-new game doesn't touch a single line of existing code; you write one class and it plugs straight into the menu. The system scales to as many games as I want to add.

The same idea runs through the rest of it: a Pile base class branches into Tableau, Foundation, Stock and Waste piles, and a Player base splits into Human and AI.

