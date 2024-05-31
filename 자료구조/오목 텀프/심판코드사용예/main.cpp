#include<iostream>
#include "Judgment.h"

#include "2020180001.h"
#include "2020182001.h"
using namespace std;

int main(void)
{
	Judgment judgment;

	judgment.SetYourFunc(
		BlackAttack_2020180001/*선공*/,
		BlackDefence_2020180001,
		WhiteAttack_2020182001/*후공*/,
		WhiteDefence_2020182001
		);
	judgment.GamePlay();

}