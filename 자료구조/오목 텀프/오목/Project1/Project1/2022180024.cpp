#include "2022180024.h"

Point p[COL][LOW];
int defense_threshold = 3; // 3개 이상 연속되는 돌을 막기 위한 임계값

void initialize_board() {
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            p[i][j] = { i, j, EMPTY };
        }
    }
}

bool is_valid_move(int x, int y, int shape) {
    if (x < 0 || x >= COL || y < 0 || y >= LOW || p[x][y].shape != EMPTY) {
        return false;
    }

    if (shape == BLACK) {
        if (is_double_three(x, y, shape) || is_double_four(x, y, shape)) {
            return false;
        }
    }

    return true;
}

bool is_winning_move(int x, int y, int shape) {
    const int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    for (auto& dir : directions) {
        int count = 1;
        for (int i = 1; i < 5; ++i) {
            int nx = x + dir[0] * i;
            int ny = y + dir[1] * i;
            if (nx >= 0 && nx < COL && ny >= 0 && ny < LOW && p[nx][ny].shape == shape) {
                count++;
            }
            else {
                break;
            }
        }
        for (int i = 1; i < 5; ++i) {
            int nx = x - dir[0] * i;
            int ny = y - dir[1] * i;
            if (nx >= 0 && nx < COL && ny >= 0 && ny < LOW && p[nx][ny].shape == shape) {
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

bool is_double_three(int x, int y, int shape) {
    int three_count = 0;
    const int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };

    for (auto& dir : directions) {
        int count = 0;
        bool open_ends = false;
        for (int i = -4; i <= 4; ++i) {
            int nx = x + dir[0] * i;
            int ny = y + dir[1] * i;
            if (nx >= 0 && nx < COL && ny >= 0 && ny < LOW) {
                if (p[nx][ny].shape == shape) {
                    count++;
                }
                else if (p[nx][ny].shape == EMPTY) {
                    if (count == 3) {
                        open_ends = true;
                        break;
                    }
                    count = 0;
                }
                else {
                    count = 0;
                }
            }
        }
        if (open_ends) {
            three_count++;
        }
        if (three_count >= 2) {
            return true;
        }
    }
    return false;
}

bool is_double_four(int x, int y, int shape) {
    int four_count = 0;
    const int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };

    for (auto& dir : directions) {
        int count = 0;
        for (int i = -4; i <= 4; ++i) {
            int nx = x + dir[0] * i;
            int ny = y + dir[1] * i;
            if (nx >= 0 && nx < COL && ny >= 0 && ny < LOW) {
                if (p[nx][ny].shape == shape) {
                    count++;
                }
                else if (p[nx][ny].shape == EMPTY) {
                    if (count == 4) {
                        four_count++;
                        break;
                    }
                    count = 0;
                }
                else {
                    count = 0;
                }
            }
        }
        if (four_count >= 2) {
            return true;
        }
    }
    return false;
}

int evaluate_board(int shape) {
    int score = 0;
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (p[i][j].shape == shape && is_winning_move(i, j, shape)) {
                score += 100;
            }
            else if (p[i][j].shape == -shape && is_winning_move(i, j, -shape)) {
                score -= 100;
            }
        }
    }
    return score;
}

void find_defensive_move(int* x, int* y, int shape) {
    const int opponent_shape = -shape;
    const int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (p[i][j].shape == opponent_shape) {
                for (auto& dir : directions) {
                    int count = 1;
                    int start_x = i;
                    int start_y = j;
                    int end_x = i;
                    int end_y = j;
                    for (int k = 1; k < 5; ++k) {
                        int nx = i + dir[0] * k;
                        int ny = j + dir[1] * k;
                        if (nx >= 0 && nx < COL && ny >= 0 && ny < LOW && p[nx][ny].shape == opponent_shape) {
                            count++;
                            end_x = nx;
                            end_y = ny;
                        }
                        else {
                            break;
                        }
                    }
                    if (count >= defense_threshold) { // 3개 이상 연속되면 방어
                        int possible_moves[2][2] = { {start_x - dir[0], start_y - dir[1]}, {end_x + dir[0], end_y + dir[1]} };
                        for (int m = 0; m < 2; ++m) {
                            int px = possible_moves[m][0];
                            int py = possible_moves[m][1];
                            if (px >= 0 && px < COL && py >= 0 && py < LOW && p[px][py].shape == EMPTY) {
                                *x = px;
                                *y = py;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void find_offensive_move(int* x, int* y, int shape) {
    const int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (p[i][j].shape == shape) {
                for (auto& dir : directions) {
                    int count = 1;
                    for (int k = 1; k < 5; ++k) {
                        int nx = i + dir[0] * k;
                        int ny = j + dir[1] * k;
                        if (nx >= 0 && nx < COL && ny >= 0 && ny < LOW && p[nx][ny].shape == shape) {
                            count++;
                        }
                        else {
                            break;
                        }
                    }
                    if (count >= 3) {
                        for (int k = 1; k < 5; ++k) {
                            int nx = i + dir[0] * k;
                            int ny = j + dir[1] * k;
                            if (nx >= 0 && nx < COL && ny >= 0 && ny < LOW && p[nx][ny].shape == EMPTY) {
                                *x = nx;
                                *y = ny;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void find_best_move(int* x, int* y, int shape) {
    // 방어 우선
    *x = -1;
    *y = -1;
    find_defensive_move(x, y, shape);
    if (*x != -1 && *y != -1) {
        return;
    }
    // 공격 시도
    find_offensive_move(x, y, shape);
    if (*x != -1 && *y != -1) {
        return;
    }
    // 중앙에 가까운 위치에 돌을 둠
    int best_x = COL / 2, best_y = LOW / 2;
    int min_distance = COL * COL + LOW * LOW;
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (is_valid_move(i, j, shape)) {
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

void WhiteAttack_2022180024(int* x, int* y) {
    find_best_move(x, y, WHITE);
    p[*x][*y].shape = WHITE; // Update the board with the move
    cout << "2022180024 흰색 둔곳: (" << *x << ", " << *y << ")" << endl;
}

void WhiteDefence_2022180024(int x, int y) {
    p[x][y].shape = WHITE;
    cout << "                                        2022180024 WhiteDefence: (" << x << ", " << y << ")" << endl;
}

void BlackAttack_2022180024(int* x, int* y) {
    find_best_move(x, y, BLACK);
    p[*x][*y].shape = BLACK; // Update the board with the move
    cout << "2022180024 검은돌 둔곳: (" << *x << ", " << *y << ")" << endl;
}

void BlackDefence_2022180024(int x, int y) {
    p[x][y].shape = BLACK;
    cout << "                                        2022180024 BlackDefence: (" << x << ", " << y << ")" << endl;
}

void init_game() {
    srand(time(NULL));
    initialize_board();
}
