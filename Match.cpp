#include "raylib.h"
#include "Match.h"
#include "setup.h"

Match::Match() : human("Player") {
    rows = 4;
    cols = 4;
    firstpick = -1;
    secondpick = -1;
    previewing = false;
    previewstart = 0.0;
    attempts = 0;
    pairsfound = 0;
    won = false;
    bnew = { 600.0f, 15.0f, 130.0f, 30.0f };
}

void Match::buildboard() {
    board.clear();
    matched.clear();

    int totalpairs = (rows * cols) / 2;
    int r;
    for (r = 1; r <= totalpairs; r++) {
        board.push_back(Card(hearts, r));
        board.push_back(Card(spades, r));
    }

    int n = (int)board.size();
    int i;
    for (i = n - 1; i > 0; i--) {
        int j = GetRandomValue(0, i);
        Card temp = board[i];
        board[i] = board[j];
        board[j] = temp;
    }

    for (i = 0; i < n; i++) {
        board[i].setface(false);
        matched.push_back(false);
    }

    firstpick = -1;
    secondpick = -1;
    previewing = false;
    attempts = 0;
    pairsfound = 0;
    won = false;
}

void Match::cellposition(int index, int& outx, int& outy) {
    int gapx = 30;
    int gapy = 25;
    int gridw = cols * cardw + (cols - 1) * gapx;
    int gridh = rows * cardh + (rows - 1) * gapy;
    int startx = (windoww - gridw) / 2;
    int starty = (windowh - gridh) / 2 + 20;

    int c = index % cols;
    int r = index / cols;
    outx = startx + c * (cardw + gapx);
    outy = starty + r * (cardh + gapy);
}

int Match::cardatposition(Vector2 m) {
    int i;
    for (i = 0; i < (int)board.size(); i++) {
        int x;
        int y;
        cellposition(i, x, y);
        Rectangle r = { (float)x, (float)y, (float)cardw, (float)cardh };
        if (CheckCollisionPointRec(m, r)) return i;
    }
    return -1;
}

void Match::start() {
    buildboard();
    human.addplayed();
}

void Match::update() {
    if (previewing) {
        if (GetTime() - previewstart > 0.8) {
            board[firstpick].setface(false);
            board[secondpick].setface(false);
            firstpick = -1;
            secondpick = -1;
            previewing = false;
        }
        return;
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;

    Vector2 m = GetMousePosition();

    if (CheckCollisionPointRec(m, bnew)) {
        buildboard();
        return;
    }

    if (won) return;

    int index = cardatposition(m);
    if (index < 0) return;
    if (matched[index]) return;
    if (index == firstpick) return;

    board[index].setface(true);

    if (firstpick == -1) {
        firstpick = index;
        return;
    }

    secondpick = index;
    attempts++;

    if (board[firstpick].getrank() == board[secondpick].getrank()) {
        matched[firstpick] = true;
        matched[secondpick] = true;
        pairsfound++;
        firstpick = -1;
        secondpick = -1;
        if (pairsfound == (rows * cols) / 2) {
            won = true;
            human.addwon();
        }
    }
    else {
        previewing = true;
        previewstart = GetTime();
    }
}

void Match::drawbutton(Rectangle r, const char* label) {
    DrawRectangleRec(r, LIGHTGRAY);
    DrawRectangleLinesEx(r, 1, DARKGRAY);
    int fs = 16;
    int tw = MeasureText(label, fs);
    DrawText(label, (int)(r.x + (r.width - tw) / 2), (int)(r.y + (r.height - fs) / 2), fs, BLACK);
}

void Match::draw() {
    DrawText("MATCH  -  Memory Game", margin, 15, 28, RAYWHITE);
    drawbutton(bnew, "New Game");

    DrawText(TextFormat("Pairs found: %d / %d", pairsfound, (rows * cols) / 2), margin, 55, 20, RAYWHITE);
    DrawText(TextFormat("Tries: %d", attempts), 300, 55, 20, RAYWHITE);

    int i;
    for (i = 0; i < (int)board.size(); i++) {
        int x;
        int y;
        cellposition(i, x, y);
        board[i].draw(x, y);
        if (matched[i]) {
            Rectangle r = { (float)x - 2, (float)y - 2, (float)cardw + 4, (float)cardh + 4 };
            DrawRectangleLinesEx(r, 3, GREEN);
        }
    }

    DrawText("Flip two cards. Same rank stays up. Match all pairs to win.",
        margin, windowh - 30, 18, RAYWHITE);

    if (won) {
        DrawRectangle(0, 0, windoww, windowh, Fade(BLACK, 0.6f));
        const char* msg = "YOU WIN!";
        int fs = 56;
        int tw = MeasureText(msg, fs);
        DrawText(msg, (windoww - tw) / 2, windowh / 2 - 60, fs, GOLD);
        const char* sub = TextFormat("Finished in %d tries.", attempts);
        int tw2 = MeasureText(sub, 22);
        DrawText(sub, (windoww - tw2) / 2, windowh / 2 + 10, 22, RAYWHITE);
    }
}

bool Match::finished() {
    return won;
}
