#include "2022180024.h"

Point p[COL][LOW];

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

    // °ËÀº µ¹ÀÏ °æ¿ì »ï»ï ¹× »ç»ç ±Ý¼ö Ã¼Å©
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

int minimax(int depth, bool is_maximizing, int alpha, int beta, int shape) {
    int score = evaluate_board(shape);
    if (depth == 0 || abs(score) >= 100) {
        return score;
    }

    if (is_maximizing) {
        int max_eval = -10000;
        for (int i = 0; i < COL; ++i) {
            for (int j = 0; j < LOW; ++j) {
                if (is_valid_move(i, j, shape)) {
                    p[i][j].shape = shape;
                    int eval = minimax(depth - 1, false, alpha, beta, shape);
                    p[i][j].shape = EMPTY;
                    max_eval = max(max_eval, eval);
                    alpha = max(alpha, eval);
                    if (beta <= alpha) {
                        return max_eval;
                    }
                }
            }
        }
        return max_eval;
    }
    else {
        int min_eval = 10000;
        int opponent_shape = -shape;
        for (int i = 0; i < COL; ++i) {
            for (int j = 0; j < LOW; ++j) {
                if (is_valid_move(i, j, opponent_shape)) {
                    p[i][j].shape = opponent_shape;
                    int eval = minimax(depth - 1, true, alpha, beta, shape);
                    p[i][j].shape = EMPTY;
                    min_eval = min(min_eval, eval);
                    beta = min(beta, eval);
                    if (beta <= alpha) {
                        return min_eval;
                    }
                }
            }
        }
        return min_eval;
    }
}

void find_best_move(int* x, int* y, int shape) {
    int best_score = -10000;
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (is_valid_move(i, j, shape)) {
                p[i][j].shape = shape;
                int score = minimax(2, false, -10000, 10000, shape); // ±íÀÌ¸¦ 2·Î ¼³Á¤
                p[i][j].shape = EMPTY;
                if (score > best_score) {
                    best_score = score;
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
    cout << "2022180024 Èò»ö µÐ°÷: (" << *x << ", " << *y << ")" << endl;
}

void WhiteDefence_2022180024(int x, int y) {
    p[x][y].shape = WHITE;
    cout << "                                        2022180024 WhiteDefence: (" << x << ", " << y << ")" << endl;
}

void BlackAttack_2022180024(int* x, int* y) {
    find_best_move(x, y, BLACK);
    p[*x][*y].shape = BLACK; // Update the board with the move
    cout << "2022180024 °ËÀºµ¹ µÐ°÷: (" << *x << ", " << *y << ")" << endl;
}

void BlackDefence_2022180024(int x, int y) {
    p[x][y].shape = BLACK;
    cout << "                                       2022180024 BlackDefence: (" << x << ", " << y << ")" << endl;
}

void init_game() {
    srand(time(NULL));
    initialize_board();
}
