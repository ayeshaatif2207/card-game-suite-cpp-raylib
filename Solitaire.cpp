#include <fstream>
#include "raylib.h"
#include "Solitaire.h"
#include "setup.h"
using namespace std;

void writepile(ofstream& out, Pile& p) {
    out << p.count() << "\n";
    int i;
    for (i = 0; i < p.count(); i++) {
        Card c = p.cardat(i);
        out << (int)c.getsuit() << " " << c.getrank() << " " << (c.isfaceup() ? 1 : 0) << "\n";
    }
}

void readpile(ifstream& in, Pile& p) {
    p.clear();
    int many = 0;
    in >> many;
    int i;
    for (i = 0; i < many; i++) {
        int s = 0;
        int r = 0;
        int f = 0;
        in >> s >> r >> f;
        Card c((Suit)s, r);
        c.setface(f == 1);
        p.addcard(c);
    }
}

void copyinto(Pile& p, vector<Card>& saved) {
    p.clear();
    int i;
    for (i = 0; i < (int)saved.size(); i++) {
        p.addcard(saved[i]);
    }
}

Solitaire::Solitaire() : player("Player") {
    picked = 0;
    pickedindex = 0;
    moves = 0;
    won = false;

    bnew = { 30.0f, 15.0f, 100.0f, 30.0f };
    bsave = { 140.0f, 15.0f, 90.0f, 30.0f };
    bload = { 240.0f, 15.0f, 90.0f, 30.0f };
    bundo = { 340.0f, 15.0f, 90.0f, 30.0f };

    loadstats();
}

void Solitaire::placepiles() {
    stock.setpos(margin + 0 * colstep, toprowy);
    waste.setpos(margin + 1 * colstep, toprowy);

    int i;
    for (i = 0; i < 4; i++) {
        foundations[i].setpos(margin + (3 + i) * colstep, toprowy);
    }
    for (i = 0; i < 7; i++) {
        tableau[i].setpos(margin + i * colstep, tableauy);
    }
}

void Solitaire::newgame() {
    stock.clear();
    waste.clear();
    int i;
    for (i = 0; i < 4; i++) foundations[i].clear();
    for (i = 0; i < 7; i++) tableau[i].clear();

    deck.build();
    deck.shuffle();
    deal();

    picked = 0;
    pickedindex = 0;
    moves = 0;
    won = false;
    history.clear();

    player.addplayed();
    savestats();
}

void Solitaire::deal() {
    int col;
    int n;
    for (col = 0; col < 7; col++) {
        for (n = 0; n <= col; n++) {
            Card c = deck.drawcard();
            if (n == col) c.setface(true);
            else c.setface(false);
            tableau[col].addcard(c);
        }
    }
    while (!deck.isempty()) {
        Card c = deck.drawcard();
        c.setface(false);
        stock.addcard(c);
    }
}

bool Solitaire::isfoundation(Pile* p) {
    int i;
    for (i = 0; i < 4; i++) {
        if (p == &foundations[i]) return true;
    }
    return false;
}

bool Solitaire::istableau(Pile* p) {
    int i;
    for (i = 0; i < 7; i++) {
        if (p == &tableau[i]) return true;
    }
    return false;
}

void Solitaire::update() {
    Vector2 m = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(m, bnew)) { newgame(); return; }
        if (CheckCollisionPointRec(m, bsave)) { savegame(); return; }
        if (CheckCollisionPointRec(m, bload)) { loadgame(); return; }
        if (CheckCollisionPointRec(m, bundo)) { undo(); return; }
        clickleft(m);
    }
}

void Solitaire::clickleft(Vector2 m) {
    Rectangle stockrect = { (float)stock.getx(), (float)stock.gety(), (float)cardw, (float)cardh };
    if (CheckCollisionPointRec(m, stockrect)) {
        drawstock();
        picked = 0;
        return;
    }

    if (picked == 0) {
        int index = 0;
        Pile* clicked = findcard(m, index);
        if (clicked != 0) {
            picked = clicked;
            pickedindex = index;
        }
    }
    else {
        Pile* dest = finddest(m);
        if (dest != 0 && dest != picked) {
            trymove(dest);
        }
        picked = 0;
    }
}

void Solitaire::drawstock() {
    savestate();
    if (!stock.isempty()) {
        Card c = stock.removetop();
        c.setface(true);
        waste.addcard(c);
    }
    else {
        while (!waste.isempty()) {
            Card c = waste.removetop();
            c.setface(false);
            stock.addcard(c);
        }
    }
    moves++;
}

Pile* Solitaire::findcard(Vector2 m, int& index) {
    if (!waste.isempty()) {
        Rectangle r = { (float)waste.getx(), (float)waste.gety(), (float)cardw, (float)cardh };
        if (CheckCollisionPointRec(m, r)) {
            index = waste.count() - 1;
            return &waste;
        }
    }

    int t;
    int i;
    for (t = 0; t < 7; t++) {
        for (i = tableau[t].count() - 1; i >= 0; i--) {
            float cx = (float)tableau[t].getx();
            float cy = (float)tableau[t].gety() + i * fany;
            float h = (float)fany;
            if (i == tableau[t].count() - 1) h = (float)cardh;
            Rectangle r = { cx, cy, (float)cardw, h };
            if (CheckCollisionPointRec(m, r)) {
                if (tableau[t].cardat(i).isfaceup()) {
                    index = i;
                    return &tableau[t];
                }
                return 0;
            }
        }
    }
    return 0;
}

Pile* Solitaire::finddest(Vector2 m) {
    int i;
    for (i = 0; i < 4; i++) {
        Rectangle r = { (float)foundations[i].getx(), (float)foundations[i].gety(), (float)cardw, (float)cardh };
        if (CheckCollisionPointRec(m, r)) return &foundations[i];
    }

    int t;
    for (t = 0; t < 7; t++) {
        float colh = (float)cardh;
        if (tableau[t].count() > 1) colh = (tableau[t].count() - 1) * (float)fany + (float)cardh;
        Rectangle r = { (float)tableau[t].getx(), (float)tableau[t].gety(), (float)cardw, colh };
        if (CheckCollisionPointRec(m, r)) return &tableau[t];
    }
    return 0;
}

bool Solitaire::goodrun(Pile* p, int start) {
    int i;
    for (i = start; i < p->count() - 1; i++) {
        Card upper = p->cardat(i);
        Card lower = p->cardat(i + 1);
        bool oppositecolour = (upper.isred() != lower.isred());
        bool onelower = (lower.getrank() == upper.getrank() - 1);
        if (!oppositecolour || !onelower) return false;
    }
    return true;
}

void Solitaire::trymove(Pile* dest) {
    int many = picked->count() - pickedindex;

    if (isfoundation(dest) && many != 1) return;

    if (many > 1) {
        if (!istableau(dest)) return;
        if (!goodrun(picked, pickedindex)) return;
    }

    Card bottom = picked->cardat(pickedindex);
    if (!dest->canplace(bottom)) return;

    savestate();

    vector<Card> moving;
    int i;
    for (i = pickedindex; i < picked->count(); i++) {
        moving.push_back(picked->cardat(i));
    }
    while (picked->count() > pickedindex) {
        picked->removetop();
    }
    for (i = 0; i < (int)moving.size(); i++) {
        dest->addcard(moving[i]);
    }

    if (istableau(picked) && !picked->isempty() && !picked->topcard().isfaceup()) {
        picked->cardat(picked->count() - 1).flip();
    }

    moves++;
    checkwin();
}

void Solitaire::checkwin() {
    int total = 0;
    int i;
    for (i = 0; i < 4; i++) {
        total += foundations[i].count();
    }
    if (total == 52 && !won) {
        won = true;
        player.addwon();
        savestats();
    }
}

void Solitaire::savestate() {
    Snapshot s;
    int i;
    int k;
    for (i = 0; i < stock.count(); i++){
        s.stock.push_back(stock.cardat(i));
    }
    for (i = 0; i < waste.count(); i++) {
        s.waste.push_back(waste.cardat(i));
    }
    for (k = 0; k < 4; k++) {
        for (i = 0; i < foundations[k].count(); i++) {
            s.foundations[k].push_back(foundations[k].cardat(i));
        }
    }
    for (k = 0; k < 7; k++) {
        for (i = 0; i < tableau[k].count(); i++) {
            s.tableau[k].push_back(tableau[k].cardat(i));
        }
    }
    s.moves = moves;
    s.won = won;
    history.push_back(s);
}

void Solitaire::undo() {
    if (history.size() == 0) return;

    Snapshot s = history[history.size() - 1];
    history.pop_back();

    copyinto(stock, s.stock);
    copyinto(waste, s.waste);
    int k;
    for (k = 0; k < 4; k++) copyinto(foundations[k], s.foundations[k]);
    for (k = 0; k < 7; k++) copyinto(tableau[k], s.tableau[k]);

    moves = s.moves;
    won = s.won;
    picked = 0;
}

void Solitaire::savegame() {
    ofstream out("savegame.txt");
    if (!out) return;

    out << moves << "\n";
    out << (won ? 1 : 0) << "\n";

    writepile(out, stock);
    writepile(out, waste);
    int i;
    for (i = 0; i < 4; i++) writepile(out, foundations[i]);
    for (i = 0; i < 7; i++) writepile(out, tableau[i]);
}

void Solitaire::loadgame() {
    ifstream in("savegame.txt");
    if (!in) return;

    int w = 0;
    in >> moves;
    in >> w;
    won = (w == 1);

    readpile(in, stock);
    readpile(in, waste);
    int i;
    for (i = 0; i < 4; i++) readpile(in, foundations[i]);
    for (i = 0; i < 7; i++) readpile(in, tableau[i]);

    picked = 0;
    history.clear();
}

void Solitaire::loadstats() {
    ifstream in("stats.txt");
    if (!in) return;
    int played = 0;
    int wins = 0;
    in >> played >> wins;
    player.setstats(played, wins);
}

void Solitaire::savestats() {
    ofstream out("stats.txt");
    if (!out) return;
    out << player.getplayed() << " " << player.getwon() << "\n";
}

void Solitaire::drawbutton(Rectangle r, const char* label) {
    DrawRectangleRec(r, LIGHTGRAY);
    DrawRectangleLinesEx(r, 1, DARKGRAY);
    int fs = 16;
    int tw = MeasureText(label, fs);
    DrawText(label, (int)(r.x + (r.width - tw) / 2), (int)(r.y + (r.height - fs) / 2), fs, BLACK);
}

void Solitaire::drawpicked() {
    if (picked == 0) return;

    float px = (float)picked->getx();
    float py = (float)picked->gety();
    float w = (float)cardw;
    float h = (float)cardh;

    if (istableau(picked)) {
        int many = picked->count() - pickedindex;
        py = (float)picked->gety() + pickedindex * fany;
        h = (many - 1) * (float)fany + (float)cardh;
    }
    Rectangle r = { px - 2, py - 2, w + 4, h + 4 };
    DrawRectangleLinesEx(r, 3, GOLD);
}

void Solitaire::draw() {
    drawbutton(bnew, "New Game");
    drawbutton(bsave, "Save");
    drawbutton(bload, "Load");
    drawbutton(bundo, "Undo");

    DrawText(TextFormat("Moves: %d", moves), 450, 12, 18, RAYWHITE);
    DrawText(TextFormat("Won %d / Played %d", player.getwon(), player.getplayed()), 450, 32, 18, RAYWHITE);

    stock.draw();
    waste.draw();
    int i;
    for (i = 0; i < 4; i++) foundations[i].draw();
    for (i = 0; i < 7; i++) tableau[i].draw();

    drawpicked();

    DrawText("Left-click: pick then place. Click stock to draw.",
        margin, windowh - 26, 16, RAYWHITE);

    if (won) {
        DrawRectangle(0, 0, windoww, windowh, Fade(BLACK, 0.6f));
        const char* msg = "YOU WIN!";
        int fs = 60;
        int tw = MeasureText(msg, fs);
        DrawText(msg, (windoww - tw) / 2, windowh / 2 - 60, fs, GOLD);
        const char* sub = "Click New Game to play again.";
        int tw2 = MeasureText(sub, 22);
        DrawText(sub, (windoww - tw2) / 2, windowh / 2 + 10, 22, RAYWHITE);
    }
}

bool Solitaire::finished() {
    return won;
}

void Solitaire::start() {
    placepiles();
    newgame();
}