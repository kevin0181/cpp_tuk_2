#include "2022180024_1.h"

Point p1[COL][LOW];

void initialize_board_1() {
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            p1[i][j] = { i, j, 0 };
        }
    }
}

bool is_valid_move_1(int x, int y) {
    return x >= 0 && x < COL && y >= 0 && y < LOW && p1[x][y].shape == 0;
}

bool is_winning_move_1(int x, int y, int shape) {
    const int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    for (auto& dir : directions) {
        int count = 1;
        for (int i = 1; i < 5; ++i) {
            int nx = x + dir[0] * i;
            int ny = y + dir[1] * i;
            if (nx >= 0 && nx < COL && ny >= 0 && ny < LOW && p1[nx][ny].shape == shape) {
                count++;
            }
            else {
                break;
            }
        }
        for (int i = 1; i < 5; ++i) {
            int nx = x - dir[0] * i;
            int ny = y - dir[1] * i;
            if (nx >= 0 && nx < COL && ny >= 0 && ny < LOW && p1[nx][ny].shape == shape) {
                count++;
            }
            else {
                break;
            }
        }
        if (count >= 5) return true;
    }
    return false;
}

void make_random_move_1(int* x, int* y) {
    do {
        *x = rand() % COL;
        *y = rand() % LOW;
    } while (!is_valid_move_1(*x, *y));
}

void WhiteAttack_2022180024_1(int* x, int* y) {
    // White attack strategy
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (is_valid_move_1(i, j)) {
                p1[i][j].shape = WHITE;
                if (is_winning_move_1(i, j, WHITE)) {
                    *x = i;
                    *y = j;
                    p1[i][j].shape = 0; // Undo the move
                    return;
                }
                p1[i][j].shape = 0; // Undo the move
            }
        }
    }
    make_random_move_1(x, y); // If no winning move found, make a random move
}

void WhiteDefence_2022180024_1(int x, int y) {
    // White defence strategy
    p1[x][y].shape = WHITE;
}

void BlackAttack_2022180024_1(int* x, int* y) {
    // Black attack strategy
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (is_valid_move_1(i, j)) {
                p1[i][j].shape = BLACK;
                if (is_winning_move_1(i, j, BLACK)) {
                    *x = i;
                    *y = j;
                    p1[i][j].shape = 0; // Undo the move
                    return;
                }
                p1[i][j].shape = 0; // Undo the move
            }
        }
    }
    make_random_move_1(x, y); // If no winning move found, make a random move
}

void BlackDefence_2022180024_1(int x, int y) {
    // Black defence strategy
    p1[x][y].shape = BLACK;
}

void init_game_1() {
    srand(time(NULL));
    initialize_board_1();
}
