#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

#define COL 19
#define LOW 19

// �� 1
// ������ -1
#define BLACK -1
#define WHITE 1
#define EMPTY 0

#undef max
#undef min

#ifndef POINT_STRUCT
#define POINT_STRUCT
struct Point {
    int x;
    int y;
    int shape;
};
#endif

extern Point p[COL][LOW];
extern int search_depth;  // Ž�� ���̸� �����ϴ� ����

void initialize_board();
bool is_valid_move(int x, int y, int shape);
bool is_winning_move(int x, int y, int shape);
bool is_double_three(int x, int y, int shape);
bool is_double_four(int x, int y, int shape);
int evaluate_board(int shape);
int minimax(int depth, bool is_maximizing, int alpha, int beta, int shape);
void find_best_move(int* x, int* y, int shape);
void WhiteAttack_2022180024(int* x, int* y);
void WhiteDefence_2022180024(int x, int y);
void BlackAttack_2022180024(int* x, int* y);
void BlackDefence_2022180024(int x, int y);
void init_game();
vector<Point> get_candidate_moves();
