#include "2022180024.h"
#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

Point p[COL][LOW];
int search_depth = 1000; // MCTS ½Ã¹Ä·¹ÀÌ¼Ç ¼ö
int defense_threshold = 3;

struct Node {
    int x, y;
    int wins, visits;
    vector<Node> children;
};

void initialize_board() {
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            p[i][j] = { i, j, EMPTY };
        }
    }
}

bool is_valid_move(int x, int y) {
    return x >= 0 && x < COL && y >= 0 && y < LOW && p[x][y].shape == EMPTY;
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

void apply_move(int x, int y, int shape) {
    p[x][y].shape = shape;
}

void undo_move(int x, int y) {
    p[x][y].shape = EMPTY;
}

int simulate(int shape) {
    vector<Point> moves;
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (is_valid_move(i, j)) {
                moves.push_back({ i, j, shape });
            }
        }
    }
    if (moves.empty()) return 0;
    random_shuffle(moves.begin(), moves.end());
    for (auto& move : moves) {
        apply_move(move.x, move.y, move.shape);
        if (is_winning_move(move.x, move.y, move.shape)) {
            undo_move(move.x, move.y);
            return 1;
        }
        undo_move(move.x, move.y);
    }
    return 0;
}

void expand_node(Node& node, int shape) {
    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (is_valid_move(i, j)) {
                node.children.push_back({ i, j, 0, 0, {} });
            }
        }
    }
}

Node* select_best_node(Node& node) {
    Node* best_node = nullptr;
    double best_value = -1.0;
    for (auto& child : node.children) {
        double uct_value = (child.wins / (double)(child.visits + 1)) +
            sqrt(2 * log(node.visits + 1) / (child.visits + 1));
        if (uct_value > best_value) {
            best_value = uct_value;
            best_node = &child;
        }
    }
    return best_node;
}

void backpropagate(vector<Node*>& visited_nodes, int result) {
    for (Node* node : visited_nodes) {
        node->visits++;
        node->wins += result;
    }
}

void mcts(Node& root, int shape) {
    for (int i = 0; i < search_depth; ++i) {
        Node* node = &root;
        vector<Node*> visited_nodes;
        int current_shape = shape;
        while (!node->children.empty()) {
            node = select_best_node(*node);
            visited_nodes.push_back(node);
            apply_move(node->x, node->y, current_shape);
            current_shape = -current_shape;
        }
        if (node->visits == 0) {
            int result = simulate(current_shape);
            backpropagate(visited_nodes, result);
        }
        else {
            expand_node(*node, current_shape);
        }
        undo_move(node->x, node->y);
    }
}

void find_best_move(int* x, int* y, int shape) {
    Node root = { -1, -1, 0, 0, {} };
    expand_node(root, shape);
    mcts(root, shape);
    Node* best_node = nullptr;
    int best_visits = -1;
    for (auto& child : root.children) {
        if (child.visits > best_visits) {
            best_visits = child.visits;
            best_node = &child;
        }
    }
    if (best_node) {
        *x = best_node->x;
        *y = best_node->y;
    }
}

void WhiteAttack_2022180024(int* x, int* y) {
    find_best_move(x, y, WHITE);
    apply_move(*x, *y, WHITE);
    cout << "2022180024 Èò»ö µÐ°÷: (" << *x << ", " << *y << ")" << endl;
}

void WhiteDefence_2022180024(int x, int y) {
    apply_move(x, y, WHITE);
    cout << "                                        2022180024 WhiteDefence: (" << x << ", " << y << ")" << endl;
}

void BlackAttack_2022180024(int* x, int* y) {
    find_best_move(x, y, BLACK);
    apply_move(*x, *y, BLACK);
    cout << "2022180024 °ËÀºµ¹ µÐ°÷: (" << *x << ", " << *y << ")" << endl;
}

void BlackDefence_2022180024(int x, int y) {
    apply_move(x, y, BLACK);
    cout << "                                        2022180024 BlackDefence: (" << x << ", " << y << ")" << endl;
}

void init_game() {
    srand(time(NULL));
    initialize_board();
}
