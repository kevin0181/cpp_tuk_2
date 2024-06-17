#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

#define COL 19
#define LOW 19

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

extern Point p1[COL][LOW];
extern int search_depth_1;
extern int defense_threshold_1;

void initialize_board_1();
bool is_valid_move_1(int x, int y, int shape);
bool is_winning_move_1(int x, int y, int shape);
bool is_double_three_1(int x, int y, int shape);
bool is_double_four_1(int x, int y, int shape);
int evaluate_board_1(int shape);
void find_defensive_move_1(int* x, int* y, int shape);
void find_offensive_move_1(int* x, int* y, int shape);
void find_best_move_1(int* x, int* y, int shape);
void WhiteAttack_2022180024_1(int* x, int* y);
void WhiteDefence_2022180024_1(int x, int y);
void BlackAttack_2022180024_1(int* x, int* y);
void BlackDefence_2022180024_1(int x, int y);
void init_game_1();
