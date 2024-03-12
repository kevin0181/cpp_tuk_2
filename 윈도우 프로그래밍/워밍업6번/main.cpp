#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <cctype>
#include <ctime> // 추가: 시간 관련 라이브러리

using namespace std;

// 주어진 함수들 그대로 유지

void generateNumbersAndOperators(int& num1, int& num2, int& num3, int& num4, int& num5,
    char& op1, char& op2, char& op3, char& op4) {
    // Seed the random number generator
    srand(time(nullptr));

    // Generate random numbers between 1 and 99
    num1 = rand() % 10 + 1;
    num2 = rand() % 10 + 1;
    num3 = rand() % 10 + 1;
    num4 = rand() % 10 + 1;
    num5 = rand() % 10 + 1;

    // Generate random operators
    switch (rand() % 4) {
    case 0: op1 = '+'; break;
    case 1: op1 = '-'; break;
    case 2: op1 = '*'; break;
    case 3: op1 = '/'; break;
    }
    switch (rand() % 4) {
    case 0: op2 = '+'; break;
    case 1: op2 = '-'; break;
    case 2: op2 = '*'; break;
    case 3: op2 = '/'; break;
    }
    switch (rand() % 4) {
    case 0: op3 = '+'; break;
    case 1: op3 = '-'; break;
    case 2: op3 = '*'; break;
    case 3: op3 = '/'; break;
    }
    switch (rand() % 4) {
    case 0: op4 = '+'; break;
    case 1: op4 = '-'; break;
    case 2: op4 = '*'; break;
    case 3: op4 = '/'; break;
    }
}

int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

int applyOperation(int a, int b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    }
    return 0;
}

int evaluateExpression(int num1, int num2, int num3, int num4, int num5,
    char op1, char op2, char op3, char op4, int mode) { // mode 파라미터 추가

    string expression = to_string(num1) + op1 + to_string(num2) + op2 +
        to_string(num3) + op3 + to_string(num4) + op4 + to_string(num5);

    stack<int> values;
    stack<char> ops;

    istringstream iss(expression);
    char token;
    while (iss >> token) {
        if (isdigit(token)) { // 0~9면 실행
            int num;
            iss.putback(token);
            iss >> num;
            values.push(num);
        }
        else if (token == '(') {
            ops.push(token);
        }
        else if (token == ')') {
            while (!ops.empty() && ops.top() != '(') {
                int b = values.top(); values.pop();
                int a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(a, b, op));
            }
            if (!ops.empty()) ops.pop(); // pop '('
        }
        else if (token == '+' || token == '-' || token == '*' || token == '/') {
            while (!ops.empty() && ((mode == 1 && precedence(ops.top()) >= precedence(token)) ||
                (mode == 2) || (mode == 3 && precedence(ops.top()) < precedence(token)))) {
                int b = values.top(); values.pop();
                int a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(a, b, op));
            }
            ops.push(token);
        }
    }

    while (!ops.empty()) {
        int b = values.top(); values.pop();
        int a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOperation(a, b, op));
    }

    return values.top();
}

// 역순으로 식을 계산하는 함수
int evaluateExpressionReverse(int num1, int num2, int num3, int num4, int num5,
    char op1, char op2, char op3, char op4) {

    stack<int> values;
    stack<char> ops;

    values.push(num1);
    values.push(num2);
    values.push(num3);
    values.push(num4);
    values.push(num5);
    ops.push(op1);
    ops.push(op2);
    ops.push(op3);
    ops.push(op4);

    while (!ops.empty()) {
        int a = values.top(); values.pop();
        int b = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOperation(a, b, op));
    }

    return values.top();
}

int main() {
    int num1, num2, num3, num4, num5;
    char op1, op2, op3, op4;
    int mode; // 추가: 사용자의 모드 입력을 저장할 변수

    generateNumbersAndOperators(num1, num2, num3, num4, num5, op1, op2, op3, op4);

    while (true) {

        cout << "Numbers to calculate :         " << num1 << op1 << num2 << op2 << num3 << op3 << num4 << op4 << num5 << endl;
        cout << "1~4 중에 선택하세요 : ";
        cin >> mode;


        switch (mode) {
        case 1:
            cout << "Result: " << num1 << op1 << num2 << op2 << num3 << op3 << num4 << op4 << num5 << " = ";
            cout << evaluateExpression(num1, num2, num3, num4, num5, op1, op2, op3, op4, mode) << endl;
            break;
        case 2:
            cout << "Result: " << num1 << op1 << num2 << op2 << num3 << op3 << num4 << op4 << num5 << " = ";
            cout << evaluateExpression(num1, num2, num3, num4, num5, op1, op2, op3, op4, mode) << endl;
            break;
        case 3: 
            cout << "Result (Reverse): " << num1 << op1 << num2 << op2 << num3 << op3 << num4 << op4 << num5 << " = ";
            cout << evaluateExpressionReverse(num1, num2, num3, num4, num5, op1, op2, op3, op4) << endl;

            break;
        case 4:
            generateNumbersAndOperators(num1, num2, num3, num4, num5, op1, op2, op3, op4);
            cout << "New numbers generated." << endl;
            break;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            break;
        }
    }

    return 0;
}
