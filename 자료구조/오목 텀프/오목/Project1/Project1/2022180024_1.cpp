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

void find_best_move_1(int* x, int* y, int shape) {
    // 자신이 이길 수 있는 위치를 찾음
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (is_valid_move_1(i, j)) {
                p1[i][j].shape = shape;
                if (is_winning_move_1(i, j, shape)) {
                    *x = i;
                    *y = j;
                    p1[i][j].shape = 0; // Undo the move
                    return;
                }
                p1[i][j].shape = 0; // Undo the move
            }
        }
    }
    // 상대방이 이길 수 있는 위치를 막음
    int opponent_shape = (shape == WHITE) ? BLACK : WHITE;
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (is_valid_move_1(i, j)) {
                p1[i][j].shape = opponent_shape;
                if (is_winning_move_1(i, j, opponent_shape)) {
                    *x = i;
                    *y = j;
                    p1[i][j].shape = 0; // Undo the move
                    return;
                }
                p1[i][j].shape = 0; // Undo the move
            }
        }
    }
    // 중앙에서 가까운 위치에 돌을 둠
    int best_x = COL / 2, best_y = LOW / 2;
    int min_distance = COL * COL + LOW * LOW;
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (is_valid_move_1(i, j)) {
                int distance = (i - best_x) * (i - best_x) + (j - best_y) * (j - best_y);
                if (distance < min_distance) {
                    min_distance = distance;
                    *x = i;
                    *y = j;
                }
            }
        }
    }
}

void WhiteAttack_2022180024_1(int* x, int* y) {
    find_best_move_1(x, y, WHITE);
    p1[*x][*y].shape = WHITE; // Update the board with the move
    cout << "2022180024_1 흰색 둔곳: (" << *x << ", " << *y << ")" << endl;
}

void WhiteDefence_2022180024_1(int x, int y) {
    p1[x][y].shape = WHITE;
    cout << "                                        2022180024_1 WhiteDefence: (" << x << ", " << y << ")" << endl;
}

void BlackAttack_2022180024_1(int* x, int* y) {
    find_best_move_1(x, y, BLACK);
    p1[*x][*y].shape = BLACK; // Update the board with the move
    cout << "2022180024_1 검은돌 둔곳: (" << *x << ", " << *y << ")" << endl;
}

void BlackDefence_2022180024_1(int x, int y) {
    p1[x][y].shape = BLACK;
    cout << "                                        2022180024_1 BlackDefence: (" << x << ", " << y << ")" << endl;
}

void init_game_1() {
    srand(time(NULL));
    initialize_board_1();
}
