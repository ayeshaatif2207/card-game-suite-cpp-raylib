#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
    char name[32];
    int gamesplayed;
    int gameswon;

public:
    Player(const char* n);
    virtual ~Player();

    const char* getname();
    int getplayed();
    int getwon();
    void setstats(int played, int wins);
    void addplayed();
    void addwon();
    virtual void playturn();
};

class HumanPlayer : public Player {
public:
    HumanPlayer(const char* n);
    void playturn();
};

class AIPlayer : public Player {
public:
    AIPlayer(const char* n);
    void playturn();
};

#endif
