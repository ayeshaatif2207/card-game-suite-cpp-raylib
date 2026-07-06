#ifndef GAME_H
#define GAME_H

class Game {

public:
    virtual ~Game();
    virtual void start() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual bool finished() = 0;
    void run(int width, int height, const char* title);

};

#endif
