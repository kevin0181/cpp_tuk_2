#include <iostream>
#include "Judgment.h"
#include "2022180024.h"
#include "2022180024_1.h"

using namespace std;

int main() {
	Judgment judgment;

	judgment.SetYourFunc(
		BlackAttack_2022180024/*����*/,
		BlackDefence_2022180024,
		WhiteAttack_2022180024_1/*�İ�*/,
		WhiteDefence_2022180024_1
	);

	judgment.GamePlay();
}