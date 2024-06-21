#include <iostream>
#include "Judgment.h"
#include "2022180024.h"
#include "2018182016.h"

using namespace std;

int main() {
	Judgment judgment;

	//judgment.SetYourFunc(
	//	BlackAttack_2022180024/*선공*/,
	//	BlackDefence_2022180024,
	//	WhiteAttack_2022180024_1/*후공*/,
	//	WhiteDefence_2022180024_1
	//);

	judgment.SetYourFunc(
		
		WhiteAttack_2022180024/*후공*/,
		WhiteDefence_2022180024
	);

	judgment.GamePlay();
}