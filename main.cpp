#include<bits/stdc++.h>
using namespace std;
#define rep(i, N) for (int i = 0; i < (N); i++)
#define SIZE 8
#define WIDTH SIZE
#define HEIGHT SIZE
typedef vector<vector<char> > Graph;
typedef pair<char, char> pchar;

char d[3] = {-1, 1, 0};
//0:空 1:黒 2:白 3:黒可能 4:白可能 7:両方可能

struct OthelloBoard {
    private:
        Graph G;

    public:
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

        void canPut() {
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
                            break;
                        }
                    }
                    rep (k, 8) {
                        if (isOK(j, i, d[k / 3], d[k % 3], 2)) {
                            G[i][j] += 4;
                            break;
                        }
                    }
                }
            }
        }

        void init() {
            rep (i, HEIGHT) rep (j, WIDTH) G[i][j] = 0;
            G[(HEIGHT - 1) / 2][(WIDTH - 1) / 2] = 2;
            G[HEIGHT / 2][WIDTH / 2] = 2;
            G[HEIGHT / 2][(WIDTH - 1) / 2] = 1;
            G[(HEIGHT - 1) / 2][WIDTH / 2] = 1;
            canPut();
            return;
        }

        OthelloBoard() : G(HEIGHT, vector<char>(WIDTH, 0)) { init(); }

        void display(int col = 1) {
            rep (i, HEIGHT + 1) {
                rep (j, WIDTH + 1) {
                    if (i == 0 && j == 0) printf(" ");
                    else if (i == 0) printf("%d", j);
                    else if (j == 0) printf("%d", i);
                    else {
                        char c = G[i - 1][j - 1];
                        if (c == 1) printf("o");
                        else if (c == 2) printf("x");
                        else if (c == col + 2 || c == 7) printf("*");
                        else printf(".");
                    }
                }
                printf("\n");
            }
            return;
        }

        void copy(Graph tG) {
            G = tG;
            return;
        }

        Graph get() {
            return G;
        }

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
        bool update(int X, int Y, int col = 1) {
            if (G[Y][X] == col + 2 || G[Y][X] == 7) {
                run(X, Y, col);
                canPut();
                return true;
            }
            return false;
        }
        
        bool isEnd() {
            bool flag = true;
            rep (i, HEIGHT) {
                rep (j, WIDTH) {
                    if (G[i][j] < 1 || 3 <= G[i][j]) flag = false;
                }
            }
            return flag;
        }
};


int main() {
    OthelloBoard board;
    board.display();
    int cnt = 0;
    while (!board.isEnd()) {
        int X, Y;
        scanf("%d %d", &X, &Y);
        printf("\n");
        if (X < 1 || WIDTH < X || Y < 1 || HEIGHT < Y) continue;
        if (board.update(X - 1, Y - 1, cnt % 2 + 1)) cnt++;
        board.display(cnt % 2 + 1);
        
    }
    
}


// #include <stdio.h>
// #include <vector>
// #include <deque>
// using namespace std;
// /* 盤のサイズ */
// #define SIZE (8)
// #define WIDTH (SIZE)
// #define HEIGHT (SIZE)
// #define rep(i, (N)) for (int i = 0; i < N; i++)
// typedef pair<char, char> pint;
// using Graph = vector<vector<char>>

// /* 石を置けるかどうかの判断 */


// /* 盤を表す二次元配列 */
// int evalTable[8][8] ={
//     {120, -20, 20, 5, 5, 20, -20, 120},
//     {-20, -40, -5, -5, -5, -5, -40, -20},
//     {20, -5, 15, 3, 3, 15, -5, 20},
//     {5, -5, 3, 3, 3, 3, -5, 5},
//     {5, -5, 3, 3, 3, 3, -5, 5},
//     {20, -5, 15, 3, 3, 15, -5, 20},
//     {-20, -40, -5, -5, -5, -5, -40, -20},
//     {120, -20, 20, 5, 5, 20, -20, 120},
// };
// int d[3] = {-1, 1, 0};

// /* 盤を初期化 */
// // void init(){
// //   for (char i = 0; i < HEIGHT; i++) {
// //     for (char j = 0; j < WIDTH; j++) {
// //       board[i][j] = empty;
// //     }
// //   }

// //   /* 盤面の真ん中に石を置く */
// //   board[HEIGHT / 2][WIDTH / 2] = white;
// //   board[HEIGHT / 2 - 1][WIDTH / 2 - 1] = white;
// //   board[HEIGHT / 2 - 1][WIDTH / 2] = black;
// //   board[HEIGHT / 2][WIDTH / 2 - 1] = black;
  
// //   return;
// // }

// /* マスを表示 */
// void displaySquare(COLOR square){

//   switch(square){
//     case white:
//         /* 白色の石は "o" で表示 */
//         printf("x");
//         break;
//     case black:
//         /* 黒色の石は "*" で表示 */
//         printf("o");
//         break;
//     case empty:
//         /* 空きは " " で表示 */
//         printf(".");
//         break;
//     default:
//         printf("エラー");
//         return;
//   }
//   return; 
// }

// /* 盤を表示 */
// void display() {
//   for (char i = 0; i < HEIGHT; i++) {
//     /* 盤の横方向のマス番号を表示 */
//     if(i == 0){
//       printf(" ");
//       for (char j = 0; j < WIDTH; j++) {
//         printf("%d", j + 1);
//       }
//       printf("\n");
//     }

//     for (char j = 0; j < WIDTH; j++) {
//       /* 盤の縦方向のます番号を表示 */
//       if(j == 0){
//         printf("%d", i + 1);
//       }

//       /* 盤に置かれた石の情報を表示 */
//       displaySquare(board[i][j]);
//     }
//     printf("\n");
//   }
//   return;
// }



// bool isOK(Graph board, int X, int Y, int dx, int dy, int col) { // 1:黒 2:白
//         bool flag = false;
//         if (X < 0 || WIDTH <= X || Y < 0 || HEIGHT <= Y || board[Y][X] != (3 - col)) return false;  
//         while (0 <= X && X < WIDTH && 0 <= Y && Y < HEIGHT && (board[Y][X] == col || board[Y][X] == 3 - col)) {
//             if (board[Y][X] == col) flag = true;
//             X += dx;
//             Y += dy;
//         }
//         return flag;
//     }

// deque<Graph> que;

// // 空:0 黒:1 白:2 黒可:3 白可:4 両方可:7
// void f() {
//     vector<pint> P;
//     Graph G = que.front();
//     que.pop_front();
//     rep (i, SIZE) {
//         rep (j, SIZE) {
//             if (G[i][j] == 3) P.push_back(pint(j, i));
//         }
//     }
    
//     for (auto p: P){
//         Graph tG(SIZE, vector<char>(SIZE));
//         rep (i, SIZE) {
//             rep (j, SIZE) {
//                 if (G[i][j] >= 3) tG[i][j] = 0;
//                 else tG[i][j] = G[i][j];
//             }
//         }

//         rep (i, SIZE) {
//             rep (j, SIZE) {
//                 if (tG[i][j] != 0) continue;
//                 rep (k, 8) {
//                     isOK(tG, j, i, d[k / 3], d[k % 3], 0);
//                     isOK(tG, j, i, d[k / 3], d[k % 3], 1);
//                 }
//             }
//         }
//         que.push_back(tG);
//     }
// }

// void sim(int cnt = 3) {
//     if (cnt % 2 == 0) return;
//     rep (i, cnt) {
//         int loop = que.size();
//         if (i % 2 == 0) {
//             rep (j, loop) {
//                 f();
//             }
//         } else {

//         }
//     }
// }



// int main(void){
//   COLOR now, next;

//   /* 盤を初期化して表示 */
// //   init();
//   display();

//   /* 最初に置く石の色 */
//   now = black;

//   /* 決着がつくまで無限ループ */
  
//   return 0;
// }