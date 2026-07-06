#include "raylib.h"
#include "War.h"
#include "Deck.h"
#include "setup.h"




War::War() : human("Player"), ai("Computer") {
    phase = 0;
    roundresult = 0;
    warthisbattle = false;
    playerwins = false;

    bnew = { 600.0, 15.0, 130.0, 30.0 };
}

int War::cardvalue(Card c) {
    if (c.getrank() == 1) return 14;
    return c.getrank();

}

void War::dealnewgame() {

    playerhand.clear();
    aihand.clear();
    playerpile.clear();
    aipile.clear();
    Deck deck;
    deck.build();
    deck.shuffle();

    int i = 0;
    while (!deck.isempty()) {
        Card c = deck.drawcard();
        c.setface(false);
        if (i % 2 == 0) playerhand.push_back(c);
        else aihand.push_back(c);
        i++;
    }


    phase = 0;
    roundresult = 0;
    warthisbattle = false;
    playerwins = false;
    human.addplayed();
    ai.addplayed();
}

void War::resolvebattle() {
    playerpile.clear();
    aipile.clear();
    warthisbattle = false;

    bool decided = false;
    while (!decided) {
        if (playerhand.size() == 0) { roundresult = -1; break; }
        if (aihand.size() == 0) { roundresult = 1; break; }

        Card pc = playerhand[0];
        playerhand.erase(playerhand.begin());
        Card ac = aihand[0];
        aihand.erase(aihand.begin());


        pc.setface(true);
        ac.setface(true);
        playerpile.push_back(pc);
        aipile.push_back(ac);
        int pv = cardvalue(pc);
        int av = cardvalue(ac);

        if (pv > av) { roundresult = 1; decided = true; }
        else if (av > pv) { roundresult = -1; decided = true; }
        else {
            warthisbattle = true;

            if (playerhand.size() > 0) {
                Card d = playerhand[0];
                playerhand.erase(playerhand.begin());
                d.setface(false);
                playerpile.push_back(d);
            }

            if (aihand.size() > 0) {
                Card d = aihand[0];
                aihand.erase(aihand.begin());
                d.setface(false);
                aipile.push_back(d);
            }
        }
    }
}

void War::collecttowinner(bool toplayer) {
    int i;
    for (i = 0; i < (int)playerpile.size(); i++) {
        Card c = playerpile[i];
        c.setface(false);
        if (toplayer) playerhand.push_back(c);
        else aihand.push_back(c);
    }
    for (i = 0; i < (int)aipile.size(); i++) {
        Card c = aipile[i];
        c.setface(false);
        if (toplayer) playerhand.push_back(c);
        else aihand.push_back(c);
    }
    playerpile.clear();
    aipile.clear();
}

void War::checkend() {
    if (playerhand.size() == 0 || aihand.size() == 0) {
        phase = 2;
        playerwins = (playerhand.size() > 0);
        if (playerwins) human.addwon();
        else ai.addwon();
    }
}

void War::start() {
    dealnewgame();
}

void War::update() {
    Vector2 m = GetMousePosition();
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;

    if (CheckCollisionPointRec(m, bnew)) {
        dealnewgame();
        return;
    }

    if (phase == 0) {
        if (playerhand.size() > 0 && aihand.size() > 0) {
            resolvebattle();
            phase = 1;
        }
    }

    else if (phase == 1) {
        collecttowinner(roundresult == 1);
        phase = 0;
        checkend();
    }
}

void War::drawbutton(Rectangle r, const char* label) {
    DrawRectangleRec(r, LIGHTGRAY);

    DrawRectangleLinesEx(r, 1, DARKGRAY);
    int fs = 16;
    int tw = MeasureText(label, fs);

    DrawText(label, (int)(r.x + (r.width - tw) / 2), (int)(r.y + (r.height - fs) / 2), fs, BLACK);
}

void War::draw() {
    DrawText("WAR  vs  Computer", margin, 15, 28, RAYWHITE);
    drawbutton(bnew, "New Game");

    int deckx = margin;
    int cardx = (windoww - cardw) / 2;

    DrawText(TextFormat("Computer: %d cards", (int)aihand.size()), deckx, 70, 20, RAYWHITE);
    Card aiback(spades, 1);

    aiback.setface(false);
    aiback.draw(deckx, 100);

    DrawText(TextFormat("You: %d cards", (int)playerhand.size()), deckx, 600, 20, RAYWHITE);

    Card pback(spades, 1);
    pback.setface(false);
    pback.draw(deckx, 470);

    if (aipile.size() > 0)
        aipile[aipile.size() - 1].draw(cardx, 130);
    if (playerpile.size() > 0)
        playerpile[playerpile.size() - 1].draw(cardx, 440);

    if (warthisbattle) {
        int stake = (int)playerpile.size() + (int)aipile.size();
        const char* w = "WAR!";
        int tw = MeasureText(w, 30);
        DrawText(w, (windoww - tw) / 2, 290, 30, GOLD);


        DrawText(TextFormat("%d cards at skatee", stake), cardx - 30, 330, 18, RAYWHITE);
    }

    if (phase == 0) {

        DrawText("Click anywhere to flip the top cards.", margin, windowh - 30, 18, RAYWHITE);
    }
    else if (phase == 1) {
        const char* msg = (roundresult == 1) ? "You win this round!" : "Computer wins this round!";
        Color col = (roundresult == 1) ? GREEN : MAROON;
        int tw = MeasureText(msg, 22);
        DrawText(msg, (windoww - tw) / 2, 360, 22, col);

        DrawText("Click anywhere to collect.", margin, windowh - 30, 18, RAYWHITE);


    }


    if (phase == 2) {
        DrawRectangle(0, 0, windoww, windowh, Fade(BLACK, 0.6f));

        const char* msg = playerwins ? "YOU WON THE GAME!!" : "COMPUTER WONS THE GAME!!";
        int fs = 44;
        int tw = MeasureText(msg, fs);
        DrawText(msg, (windoww - tw) / 2, windowh / 2 - 40, fs, GOLD);

        const char* sub = "Click new game to play again.";
        int tw2 = MeasureText(sub, 22);
        DrawText(sub, (windoww - tw2) / 2, windowh / 2 + 20, 22, RAYWHITE);
    }
}

bool War::finished() {
    return phase == 2;
}
