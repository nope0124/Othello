#include<bits/stdc++.h>
using namespace std;
#define rep(i, N) for (int i = 0; i < (N); i++)
#define SIZE 8
#define WIDTH SIZE
#define HEIGHT SIZE
typedef vector<vector<char> > Graph;
typedef pair<int, int> pint;

char d[3] = {-1, 1, 0};
int evalTable[8][8] = {
    {120, -20, 20,  5,  5, 20, -20, 120},
    {-20, -40, -5, -5, -5, -5, -40, -20},
    { 20,  -5, 15,  3,  3, 15,  -5,  20},
    {  5,  -5,  3,  3,  3,  3,  -5,   5},
    {  5,  -5,  3,  3,  3,  3,  -5,   5},
    { 20,  -5, 15,  3,  3, 15,  -5,  20},
    {-20, -40, -5, -5, -5, -5, -40, -20},
    {120, -20, 20,  5,  5, 20, -20, 120},
};

//0:空 1:黒 2:白 3:黒可能 4:白可能 7:両方可能

struct OthelloBoard {
    private:
        Graph G;
        vector<pint> black;
        vector<pint> white;
        int score = 0;
        int plot;
    public:
    // 決めた方向に返せるかを判定
        bool isOK(int X, int Y, int dx, int dy, int col) { // 1:黒 2:白
            bool flag = false;
            X += dx;
            Y += dy;
            if (X < 0 || WIDTH <= X || Y < 0 || HEIGHT <= Y || G[Y][X] != (3 - col)) return false;  
            while (0 <= X && X < WIDTH && 0 <= Y && Y < HEIGHT && (G[Y][X] == col || G[Y][X] == 3 - col)) {
                if (G[Y][X] == col) flag = true;
                X += dx;
                Y += dy;
            }
            return flag;
        }

    // 置いたコマを残して盤面を再探索
        void canPut(bool isMyTurn = true) {
            black.clear();
            white.clear();
            rep (i, HEIGHT) {
                rep (j, WIDTH) {
                    if (G[i][j] == 1 || G[i][j] == 2) continue;
                    G[i][j] = 0;
                }
            }
            rep (i, HEIGHT) {
                rep (j, WIDTH) {
                    if (G[i][j] != 0) continue;
                    rep (k, 8) {
                        if (isOK(j, i, d[k / 3], d[k % 3], 1)) {
                            G[i][j] += 3;
                            int tscore = score;
                            if (isMyTurn) tscore += evalTable[i][j];
                            else tscore -= evalTable[i][j];
                            black.push_back(pint(i * WIDTH + j, tscore));
                            break;
                        }
                    }
                    rep (k, 8) {
                        if (isOK(j, i, d[k / 3], d[k % 3], 2)) {
                            G[i][j] += 4;
                            int tscore = score;
                            if (isMyTurn) tscore += evalTable[i][j];
                            else tscore -= evalTable[i][j];
                            white.push_back(pint(i * WIDTH + j, tscore));
                            break;
                        }
                    }
                }
            }
        }

    // 初期化
        void init() {
            rep (i, HEIGHT) rep (j, WIDTH) G[i][j] = 0;
            G[(HEIGHT - 1) / 2][(WIDTH - 1) / 2] = 2;
            G[HEIGHT / 2][WIDTH / 2] = 2;
            G[HEIGHT / 2][(WIDTH - 1) / 2] = 1;
            G[(HEIGHT - 1) / 2][WIDTH / 2] = 1;
            canPut();
            return;
        }

    // コンストラクタ
        OthelloBoard() : G(HEIGHT, vector<char>(WIDTH, 0)) { init(); }

    // 盤面を表示
        void display(int col = 1) {
            rep (i, HEIGHT + 1) {
                rep (j, WIDTH + 1) {
                    if (i == 0 && j == 0) printf("  ");
                    else if (i == 0) printf("%d ", j);
                    else if (j == 0) printf("%d ", i);
                    else {
                        char c = G[i - 1][j - 1];
                        if (c == 1) printf("o ");
                        else if (c == 2) printf("x ");
                        else if (c == col + 2 || c == 7) printf("* ");
                        else printf(". ");
                    }
                }
                printf("\n");
            }
            return;
        }

    // 盤面をコピー
        void copy(Graph tG, vector<pint>, vector<pint> tblack, vector<pint> twhite) {
            G = tG;
            black = tblack;
            white = twhite;
            return;
        }

    // (外部から)盤面を取得
        Graph getBoard() {
            return G;
        }

    // blackを取得
        vector<pint> getBlack() {
            return black;
        }

    // whiteを取得
        vector<pint> getWhite() {
            return white;
        }

        void setScore(int tscore) {
            score = tscore;
        }

        int getScore() {
            return score;
        }

        void setPlot(int tplot) {
            plot = tplot;
        }

        int getPlot() {
            return plot;
        }
    
    // 実際にコマを置く
        void run(int X, int Y, int col) {
            G[Y][X] = col;
            rep (k, 8) {
                if (!isOK(X, Y, d[k / 3], d[k % 3], col)) continue;
                int dx = d[k / 3];
                int dy = d[k % 3];
                int tX = X + dx;
                int tY = Y + dy;
                while (0 <= tX && tX < WIDTH && 0 <= tY && tY <= HEIGHT) {
                    if (G[tY][tX] == col) break;
                    G[tY][tX] = col;
                    tX += dx;
                    tY += dy;
                }
            }
        }

    // コマを置けるか検証   
        bool update(int X, int Y, int col = 1) {
            if (X < 0 || WIDTH <= X || Y < 0 || HEIGHT <= Y) return false;
            if (G[Y][X] == col + 2 || G[Y][X] == 7) {
                run(X, Y, col);
                canPut();
                return true;
            }
            return false;
        }
        
    // 終了判定
        bool isEnd() {
            bool flag = false;
            rep (i, HEIGHT) {
                rep (j, WIDTH) {
                    if (G[i][j] == 3 || G[i][j] == 4 || G[i][j] == 7) flag = true;
                }
            }
            return !flag;
        }
};

pint evalTableAI(OthelloBoard board, int turnCount = 7, int col = 2) {
    deque<OthelloBoard> que;
    int flag = -1;
    OthelloBoard saveBoard = board;
    que.push_back(board);
    rep (turn, turnCount) {
        int cnt = que.size();
        if (cnt == 0) {
            flag = turn;
            break;
        }
        if (turn % 2 == 0) {
            rep (i, cnt) {
                board = que.front();
                que.pop_front();
                vector<pint> P;
                if (col % 2 == 0) P = board.getWhite();
                else P = board.getBlack();
                if (turn == 0 && P.size() == 0) {
                    return pint(-1, -1);
                } 
                for (pint p: P) {
                    OthelloBoard tmp = board;
                    int X = p.first % WIDTH;
                    int Y = p.first / WIDTH;
                    if (turn == 0) tmp.setPlot(p.first);
                    tmp.setScore(p.second);
                    tmp.update(X, Y, col);
                    tmp.canPut(false);
                    que.push_back(tmp);
                    // printf("%d %d %d\n", p.first % WIDTH + 1, p.first / WIDTH + 1, p.second);
                }
            }
        } else {
            rep (i, cnt) {
                board = que.front();
                que.pop_front();
                vector<pint> P;
                if (col % 2 == 0) P = board.getBlack();
                else P = board.getWhite();
                int save = INT_MAX;
                for (pint p: P) {
                    save = min(save, p.second);
                }
                for (pint p: P) {
                    if (save < p.second) continue;
                    OthelloBoard tmp = board;
                    int X = p.first % WIDTH;
                    int Y = p.first / WIDTH;
                    tmp.setScore(save);
                    tmp.update(X, Y, 3 - col);
                    tmp.canPut(true);
                    que.push_back(tmp);
                    // printf("%d %d %d\n", p.first % WIDTH + 1, p.first / WIDTH + 1, p.second);
                }
            }
        }
        // printf("\n");
    }
    if (flag >= 0) {
        if (saveBoard.isEnd()) return pint(-1, -1);
        if (col % 2 == 0) {
            return pint(saveBoard.getWhite()[0].first % WIDTH, saveBoard.getWhite()[0].first / WIDTH);
        } else {
            return pint(saveBoard.getBlack()[0].first % WIDTH, saveBoard.getBlack()[0].first / WIDTH);
        }
    }
    int MAX = INT_MIN;
    for (OthelloBoard c: que) {
        MAX = max(MAX, c.getScore());
    }
    for (OthelloBoard c: que) {
        if (MAX == c.getScore()) {
            return pint(c.getPlot() % WIDTH, c.getPlot() / WIDTH);
        }
    }
    return pint(-1, -1);
}

int main() {
    OthelloBoard board;
    board.display();
    int cnt = 0;
    while (!board.isEnd()) {
        if (cnt % 2 == 0) {
            int X, Y;
            scanf("%d %d", &X, &Y);
            printf("\n");
            if (X < 1 || WIDTH < X || Y < 1 || HEIGHT < Y) continue;
            if (board.update(X - 1, Y - 1, cnt % 2 + 1)) cnt++;
            board.display(cnt % 2 + 1);
        } else {
            pint p = evalTableAI(board);
            printf("%d %d\n\n", p.first + 1, p.second + 1);
            board.update(p.first, p.second, 2);
            cnt++;
            board.display(1);
        }
    }
    
}