#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

#define COL 19
#define LOW 19

// »Úµπ 1
// ∞À¿∫µπ -1
#define BLACK -1
#define WHITE 1

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

extern Point p1[COL][LOW];

void initialize_board_1();
bool is_valid_move_1(int x, int y);
bool is_winning_move_1(int x, int y, int shape);
void make_random_move_1(int* x, int* y);
void WhiteAttack_2022180024_1(int* x, int* y);
void WhiteDefence_2022180024_1(int x, int y);
void BlackAttack_2022180024_1(int* x, int* y);
void BlackDefence_2022180024_1(int x, int y);
void init_game_1();
