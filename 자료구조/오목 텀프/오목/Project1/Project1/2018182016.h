#pragma once
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <deque>
#define endl '\n'
using namespace std;

const int n_2018182016 = 19;
const int BLACK_2018182016 = -1;
const int WHITE_2018182016 = 1;

int board_2018182016[n_2018182016][n_2018182016]; // 게임 보드
int w_board_2018182016[n_2018182016][n_2018182016]; // 가중치 보드

const int dir_2018182016[8][2] = { {1,0}, {0,1}, {1,1}, {1,-1}, {-1,0}, {0,-1}, {-1,-1}, {-1,1} };
const int w_2018182016[2][6] = { {0,1,50,9999,500000,10000000}, {0,1,12,250,400000,10000000} };
int w2_2018182016[2][6][3][2];
int stx_2018182016, sty_2018182016; // AI 검색 시작 좌표
int ansx_2018182016, ansy_2018182016; // 검색 결과 좌표
bool tf_2018182016;

struct Info_2018182016 {
    int x, y, weight;
};

bool compare_2018182016(const Info_2018182016& a, const Info_2018182016& b) {
    return a.weight > b.weight;
}

void init_weights_2018182016() {
    // 1개 연속일 때
    w2_2018182016[0][1][0][0] = 2; w2_2018182016[1][1][0][0] = 1;
    w2_2018182016[0][1][0][1] = 2; w2_2018182016[1][1][0][1] = 0;

    // 2개 연속일 때
    w2_2018182016[0][2][0][0] = 25; w2_2018182016[1][2][0][0] = 4;
    w2_2018182016[0][2][0][1] = 25; w2_2018182016[1][2][0][1] = 1;
    w2_2018182016[0][2][1][1] = 2; w2_2018182016[1][2][1][1] = 1;
    w2_2018182016[0][2][1][0] = 2; w2_2018182016[1][2][1][0] = 1;

    // 3개 연속일 때
    w2_2018182016[0][3][0][0] = 521; w2_2018182016[1][3][0][0] = 105;
    w2_2018182016[0][3][0][1] = 301; w2_2018182016[1][3][0][1] = 13;
    w2_2018182016[0][3][1][0] = 301; w2_2018182016[1][3][1][0] = 13;
    w2_2018182016[0][3][1][1] = 301; w2_2018182016[1][3][1][1] = 13;

    // 4개 연속일 때
    w2_2018182016[0][4][0][0] = 21000; w2_2018182016[0][4][1][0] = 20010; w2_2018182016[0][4][2][0] = 20010;
    w2_2018182016[1][4][0][0] = 4001; w2_2018182016[1][4][1][0] = 4001; w2_2018182016[1][4][2][0] = 4001;

    // 5개 연속일 때 (승리 상황)
    w2_2018182016[0][5][0][0] = 10000000; w2_2018182016[1][5][0][0] = 10000000;
}


bool check_2018182016(int color_2018182016) {
    for (int i = 0; i < n_2018182016; i++) {
        for (int j = 0; j < n_2018182016; j++) {
            if (board_2018182016[i][j] == color_2018182016) {
                for (int d = 0; d < 8; d++) {
                    int cnt_2018182016 = 1;
                    while (true) {
                        int nx_2018182016 = i + (cnt_2018182016 * dir_2018182016[d][0]);
                        int ny_2018182016 = j + (cnt_2018182016 * dir_2018182016[d][1]);
                        if (nx_2018182016 < 0 || ny_2018182016 < 0 || nx_2018182016 >= n_2018182016 || ny_2018182016 >= n_2018182016)
                            break;
                        if (board_2018182016[nx_2018182016][ny_2018182016] != color_2018182016)
                            break;
                        cnt_2018182016++;
                    }
                    if (cnt_2018182016 == 5)
                        return true;
                }
            }
        }
    }
    return false;
}



void add_weight_2018182016(int color_2018182016[2]) {
    memset(w_board_2018182016, 0, sizeof(w_board_2018182016));

    for (int type_2018182016 = 0; type_2018182016 < 2; type_2018182016++) {
        for (int i = 0; i < n_2018182016; i++) {
            for (int j = 0; j < n_2018182016; j++) {
                int sum_2018182016 = 0;
                if (board_2018182016[i][j]) continue;

                struct CountInfo {
                    int num, enemy, emptyspace;
                } Count[4];

                for (int d = 0; d < 4; d++) {
                    int nx_2018182016, ny_2018182016;
                    int cnt_2018182016 = 1;
                    int zerocnt1_2018182016 = 0;
                    int zerocnt2_2018182016 = 0;
                    int num_2018182016 = 0;
                    int enemy_cnt_2018182016 = 0;
                    int remember_2018182016 = 0;
                    int before_2018182016 = 0;

                    // 정방향 검사
                    while (true) {
                        nx_2018182016 = i + (cnt_2018182016 * dir_2018182016[d][0]);
                        ny_2018182016 = j + (cnt_2018182016 * dir_2018182016[d][1]);
                        if (nx_2018182016 < 0 || ny_2018182016 < 0 || nx_2018182016 >= n_2018182016 || ny_2018182016 >= n_2018182016) break;

                        if (board_2018182016[nx_2018182016][ny_2018182016] == color_2018182016[(type_2018182016 + 1) % 2]) {
                            if (remember_2018182016 || zerocnt1_2018182016 == 0) enemy_cnt_2018182016++;
                            break;
                        }

                        if (board_2018182016[nx_2018182016][ny_2018182016] == color_2018182016[type_2018182016]) {
                            remember_2018182016 = zerocnt1_2018182016;
                            num_2018182016++;
                        }
                        if (board_2018182016[nx_2018182016][ny_2018182016] == 0) zerocnt1_2018182016++;
                        if (zerocnt1_2018182016 >= 2) break;
                        cnt_2018182016++;
                    }

                    zerocnt1_2018182016 = remember_2018182016;
                    cnt_2018182016 = 1;
                    remember_2018182016 = 0;

                    // 반대 방향 검사
                    while (true) {
                        nx_2018182016 = i + (cnt_2018182016 * dir_2018182016[d + 4][0]);
                        ny_2018182016 = j + (cnt_2018182016 * dir_2018182016[d + 4][1]);
                        if (nx_2018182016 < 0 || ny_2018182016 < 0 || nx_2018182016 >= n_2018182016 || ny_2018182016 >= n_2018182016) break;

                        if (board_2018182016[nx_2018182016][ny_2018182016] == color_2018182016[(type_2018182016 + 1) % 2]) {
                            if (remember_2018182016 || zerocnt2_2018182016 == 0) enemy_cnt_2018182016++;
                            break;
                        }

                        if (board_2018182016[nx_2018182016][ny_2018182016] == color_2018182016[type_2018182016]) {
                            remember_2018182016 = zerocnt2_2018182016;
                            num_2018182016++;
                        }
                        if (board_2018182016[nx_2018182016][ny_2018182016] == 0) zerocnt2_2018182016++;
                        if (zerocnt2_2018182016 >= 2) break;
                        cnt_2018182016++;
                    }
                    zerocnt2_2018182016 = remember_2018182016;
                    int zerocnt_2018182016 = zerocnt1_2018182016 + zerocnt2_2018182016;
                    Count[d] = { num_2018182016, enemy_cnt_2018182016, zerocnt_2018182016 };
                }

                // 나의 돌을 상대가 막고 있음 가중치 많이 낮춰 드리자
                for (int d = 0; d < 4; d++) {
                    int num_2018182016 = Count[d].num, enemy_2018182016 = Count[d].enemy, emptyspace_2018182016 = Count[d].emptyspace;
                    int temp_w_2018182016 = w2_2018182016[(type_2018182016 + 1) % 2][num_2018182016][enemy_2018182016][emptyspace_2018182016];
                    // 빈 공간은 하나만 감당, num + emptyspace >= 5, enemy 2개 가중치 부여하지 않는다.
                    if (emptyspace_2018182016 >= 2 || num_2018182016 + emptyspace_2018182016 >= 5) continue;
                    if (num_2018182016 != 4 && enemy_2018182016 >= 2) continue;
                    sum_2018182016 += temp_w_2018182016;

                }

                w_board_2018182016[i][j] += sum_2018182016;
                if (board_2018182016[i][j]) w_board_2018182016[i][j] = 0;
            }
        }
    }
}




void search_2018182016(int cnt_2018182016, int color_2018182016) {
    int ncolor_2018182016[2] = { 0, };
    if (color_2018182016 == BLACK_2018182016) {
        ncolor_2018182016[0] = WHITE_2018182016;
        ncolor_2018182016[1] = BLACK_2018182016;
    }
    else {
        ncolor_2018182016[0] = BLACK_2018182016;
        ncolor_2018182016[1] = WHITE_2018182016;
    }

    add_weight_2018182016(ncolor_2018182016);
    deque<Info_2018182016> save_pos_2018182016;
    int high_2018182016 = 0;

    // 특정 범위 내에서만 검색
    const int range = 4; // 검색 범위 설정
    int min_x = max(0, stx_2018182016 - range);
    int max_x = min(n_2018182016 - 1, stx_2018182016 + range);
    int min_y = max(0, sty_2018182016 - range);
    int max_y = min(n_2018182016 - 1, sty_2018182016 + range);

    for (int i = min_x; i <= max_x; i++) {
        for (int j = min_y; j <= max_y; j++) {
            int wow_2018182016 = w_board_2018182016[i][j];
            if (wow_2018182016) {
                if (wow_2018182016 == 301 || wow_2018182016 == 302) wow_2018182016 = 24;
                else if (wow_2018182016 >= 118 && wow_2018182016 <= 200) wow_2018182016 = 320;
                save_pos_2018182016.push_back({ i, j, wow_2018182016 });
                high_2018182016 = max(high_2018182016, wow_2018182016);
            }
        }
    }

    sort(save_pos_2018182016.begin(), save_pos_2018182016.end(), compare_2018182016);

    if (save_pos_2018182016.empty()) return; // 추가: 비어있는 경우 예외 처리

    int MAX_2018182016 = save_pos_2018182016[0].weight;
    int idx_2018182016 = 0;
    for (int i = 1; i < save_pos_2018182016.size(); i++) {
        idx_2018182016 = i;
        int num_2018182016 = save_pos_2018182016[i].weight;
        if (num_2018182016 != MAX_2018182016) break;
    }

    save_pos_2018182016.erase(save_pos_2018182016.begin() + idx_2018182016, save_pos_2018182016.end());

    int temp_color_2018182016 = (color_2018182016 == BLACK_2018182016) ? WHITE_2018182016 : BLACK_2018182016;
    if (cnt_2018182016 % 2 == 1 && check_2018182016(temp_color_2018182016)) {
        return;
    }
    if (tf_2018182016) return;

    if (!tf_2018182016 && (cnt_2018182016 % 2 == 1 && ((MAX_2018182016 >= 326 && MAX_2018182016 < 406) || MAX_2018182016 >= 521))) {
        if (!((105 <= MAX_2018182016 && MAX_2018182016 <= 300) || (4000 <= MAX_2018182016 && MAX_2018182016 < 20000))) {
            tf_2018182016 = true;
            ansx_2018182016 = stx_2018182016;
            ansy_2018182016 = sty_2018182016;
            return;
        }
    }

    if (cnt_2018182016 == 20) { // 기존 깊이를 유지
        return;
    }

    if (color_2018182016 == BLACK_2018182016) {
        for (int i = 0; i < save_pos_2018182016.size(); i++) {
            int x_2018182016 = save_pos_2018182016[i].x;
            int y_2018182016 = save_pos_2018182016[i].y;
            board_2018182016[x_2018182016][y_2018182016] = color_2018182016;
            search_2018182016(cnt_2018182016 + 1, WHITE_2018182016);
            board_2018182016[x_2018182016][y_2018182016] = 0;
        }
    }
    else if (color_2018182016 == WHITE_2018182016) {
        for (int i = 0; i < save_pos_2018182016.size(); i++) {
            int x_2018182016 = save_pos_2018182016[i].x;
            int y_2018182016 = save_pos_2018182016[i].y;
            board_2018182016[x_2018182016][y_2018182016] = color_2018182016;
            search_2018182016(cnt_2018182016 + 1, BLACK_2018182016);
            board_2018182016[x_2018182016][y_2018182016] = 0;
        }
    }
}



void AI_2018182016(int user_color_2018182016, int ai_color_2018182016) {
    tf_2018182016 = false;
    int color_2018182016[2] = { user_color_2018182016, ai_color_2018182016 };
    memset(w_board_2018182016, 0, sizeof(w_board_2018182016));

    // Check if it's the first or second move
    int move_count = 0;
    for (int i = 0; i < n_2018182016; ++i) {
        for (int j = 0; j < n_2018182016; ++j) {
            if (board_2018182016[i][j] != 0) {
                move_count++;
            }
        }
    }

    if (move_count == 0) {
        // Place the first move at the center
        ansx_2018182016 = n_2018182016 / 2;
        ansy_2018182016 = n_2018182016 / 2;
    }
    else if (move_count == 1) {
        // Place the second move next to the center
        int center_x = n_2018182016 / 2;
        int center_y = n_2018182016 / 2;
        // Check surrounding cells to find an empty one
        if (board_2018182016[center_x - 1][center_y] == 0) {
            ansx_2018182016 = center_x - 1;
            ansy_2018182016 = center_y;
        }
        else if (board_2018182016[center_x + 1][center_y] == 0) {
            ansx_2018182016 = center_x + 1;
            ansy_2018182016 = center_y;
        }
        else if (board_2018182016[center_x][center_y - 1] == 0) {
            ansx_2018182016 = center_x;
            ansy_2018182016 = center_y - 1;
        }
        else if (board_2018182016[center_x][center_y + 1] == 0) {
            ansx_2018182016 = center_x;
            ansy_2018182016 = center_y + 1;
        }
    }
    else {
        add_weight_2018182016(color_2018182016);
        deque<Info_2018182016> save_pos_2018182016;
        int high_2018182016 = 0;
        for (int i = 0; i < n_2018182016; i++) {
            for (int j = 0; j < n_2018182016; j++) {
                int wow_2018182016 = w_board_2018182016[i][j];
                if (wow_2018182016) {
                    if (wow_2018182016 == 301 || wow_2018182016 == 302) wow_2018182016 = 24;
                    else if (wow_2018182016 >= 118 && wow_2018182016 <= 200) wow_2018182016 = 320;
                    save_pos_2018182016.push_back({ i, j, wow_2018182016 });
                    if (high_2018182016 < wow_2018182016) {
                        high_2018182016 = wow_2018182016;
                        ansx_2018182016 = i;
                        ansy_2018182016 = j;
                    }
                }
            }
        }

        sort(save_pos_2018182016.begin(), save_pos_2018182016.end(), compare_2018182016);

        if (!save_pos_2018182016.empty()) {
            int MAX_2018182016 = save_pos_2018182016[0].weight;

            if (!((MAX_2018182016 >= 326 && MAX_2018182016 < 406) || MAX_2018182016 >= 521)) {
                for (int i = 0; i < save_pos_2018182016.size(); i++) {
                    int x_2018182016 = save_pos_2018182016[i].x;
                    int y_2018182016 = save_pos_2018182016[i].y;
                    stx_2018182016 = x_2018182016;
                    sty_2018182016 = y_2018182016;
                    board_2018182016[x_2018182016][y_2018182016] = ai_color_2018182016;
                    search_2018182016(0, user_color_2018182016);
                    board_2018182016[x_2018182016][y_2018182016] = 0;
                }
            }
        }
    }

    board_2018182016[ansx_2018182016][ansy_2018182016] = ai_color_2018182016;
}



void WhiteAttack_2018182016(int* x_2018182016, int* y_2018182016) {
    init_weights_2018182016();
    AI_2018182016(BLACK_2018182016, WHITE_2018182016);
    *x_2018182016 = ansx_2018182016;
    *y_2018182016 = ansy_2018182016;
    board_2018182016[*y_2018182016][*x_2018182016] = WHITE_2018182016;
}

void BlackAttack_2018182016(int* x_2018182016, int* y_2018182016) {
    init_weights_2018182016();
    AI_2018182016(WHITE_2018182016, BLACK_2018182016);
    *x_2018182016 = ansx_2018182016;
    *y_2018182016 = ansy_2018182016;
    board_2018182016[*y_2018182016][*x_2018182016] = BLACK_2018182016;
}

void WhiteDefence_2018182016(int x_2018182016, int y_2018182016) {
    board_2018182016[y_2018182016][x_2018182016] = BLACK_2018182016;
}

void BlackDefence_2018182016(int x_2018182016, int y_2018182016) {
    board_2018182016[y_2018182016][x_2018182016] = WHITE_2018182016;
}
