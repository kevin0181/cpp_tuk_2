#include <iostream>
#include <map>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <unordered_map>

#define MAX_CLASS 13

using namespace std;

void start1(int& candidateNum, map<int, vector<int>> candidateClass, vector<int> classCnt, vector<int> voteC);
void fP(vector<int> classCnt);
void fNum(vector<int> voteC, int num, map<int, vector<int>> candidateClass);
void fA(vector<int> voteC, map<int, vector<int>> candidateClass);
void fB(vector<int> voteC, map<int, vector<int>> candidateClass);

random_device rd;
mt19937 gen(rd());

int main() {
	int candidateNum{};
	map<int, vector<int>> candidateClass; //후보 반
	vector<int> classCnt; // 대의원 반 정원
	//map<int, vector<int>> delegateClass; //대의원 반
	vector<int> voteC; // 1000명 투표

	start1(candidateNum, candidateClass, classCnt, voteC);

}

void start1(int& candidateNum, map<int, vector<int>> candidateClass, vector<int> classCnt, vector<int> voteC) {

	vector<int> candidate;

	cout << "대의원 후보자 수를 입력하세요(150~250) : ";
	cin >> candidateNum;
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 문자 여러개 입력하는거 방지 (입력된 버퍼 지움)

	if (cin.fail()) {
		// 값이 정수가 아닙니다.
		cin.clear(); // 입력 버퍼 지우기
		cin.ignore(); // 버퍼에 남아있는 문자열 무시
		start1(candidateNum, candidateClass, classCnt, voteC);
	}

	if (candidateNum > 250 || candidateNum < 150) {
		cout << "후보자의 수를 알맞게 입력해주세요." << endl;
		start1(candidateNum, candidateClass, classCnt, voteC);
	}

	// create candidate
	for (int i = 0; i < candidateNum; ++i) {
		candidate.push_back(i + 1);
	}

	// push candidate in class
	uniform_int_distribution<int> uid(1, 13);
	for (int i = 0; i < candidateNum; ++i) {
		candidateClass[uid(gen)].push_back(candidate[i]); // 후보자들을 랜덤한 반에 넣어줌.
	}

	// vote ~!
	// push candidate in class
	uniform_int_distribution<int> dis(1, candidateNum);
	for (int i = 0; i < 1000; ++i) {
		voteC.push_back(dis(gen));
	}

	for (const auto& pair : candidateClass) {
		for (size_t i = 0; i < pair.second.size(); ++i) {
			cout << pair.first << "반 - " << pair.second[i] << "번";
			if (i != pair.second.size() - 1) {
				cout << ", ";
			}
		}
		cout << endl;
	}

	while (true) {
		
		cout << endl;
		string orderS{};
		cout << "명령어를 입력하세요 : ";
		cin >> orderS;

		unordered_map<std::string, int> myMap = {
		{"p", 'p'},
		{"a", 'a'},
		{"b",'b'}
		};

			int value = myMap[orderS];
			switch (value)
			{
			case 'p': //대의원 수 출력
				fP(classCnt);
				break;
			case 'a':
				fA(voteC, candidateClass);
				break;
			case 'b':
				fB(voteC, candidateClass);
				break;
			default:
				try {
					int num = stoi(orderS);
					if (num <= 1000)
						fNum(voteC, num, candidateClass);
					else
						cout << "1000 이하로 입력해주세요." << endl;
					break;
				}
				catch (const  std::invalid_argument& e) {
					cerr << "예외 발생: 입력된 문자열에는 숫자가 아닌 문자가 포함되어 있습니다." << endl;
				}
			}

		cout << endl << "----------------------------------------" << endl << endl;
	}
}
void fB(vector<int> voteC, map<int, vector<int>> candidateClass) {

	for (auto& pair : candidateClass) {
		int key = pair.first; // 키 읽기
		vector<int>& values = pair.second; // 값(벡터) 읽기

		map<int, int> candidateList;
		int sum{};

		for (int value : values) {
			for (int i = 0; i < 1000; ++i) {
				if (voteC[i] == value) {
					++sum;
				}
			}
			candidateList[value] = sum;
			sum = 0;
		}

		vector<std::pair<int, int>> vec(candidateList.begin(), candidateList.end());

		sort(vec.begin(), vec.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
			return a.second > b.second;
			});

		cout << key << "반 : ";
		for (const auto& pair : vec) {
			cout << pair.first << "번 - " << pair.second << "표, ";
		}

		cout << endl;

	}

}
void fA(vector<int> voteC, map<int, vector<int>> candidateClass) {

	for (auto& pair : candidateClass) {
		int key = pair.first; // 키 읽기
		vector<int>& values = pair.second; // 값(벡터) 읽기

		map<int, int> candidateList;
		int sum{};

		for (int value : values) {
			for (int i = 0; i < 1000; ++i) {
				if (voteC[i] == value) {
					++sum;
				}
			}
			candidateList[value] = sum;
			sum = 0;
		}

		vector<std::pair<int, int>> vec(candidateList.begin(), candidateList.end());

		sort(vec.begin(), vec.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
			return a.second < b.second;
		});

		cout << key << "반 : ";
		for (const auto& pair : vec) {
			cout << pair.first << "번 - " << pair.second << "표, ";
		}

		cout << endl;

	}

}

void fNum(vector<int> voteC, int num, map<int, vector<int>> candidateClass) {

	int voteNum{}; // num이 투표한 사람 번호

	for (int i = 0; i < 1000; ++i) {
		if (i + 1 == num) {
			voteNum = voteC[i];
		}
	}

	int classKey{};
	int keyValue{};

	for (const auto& pair : candidateClass) {
		int key = pair.first; // 키 읽기
		const vector<int>& values = pair.second; // 값(벡터) 읽기

		for (int value : values) {
			if (voteNum == value) {
				classKey = key;
				keyValue = value;
			}
		}

	}

	cout << num << "이 투표한 사람은 " << classKey << "반 - " << keyValue << "번" << " 입니다." << endl;
}



void fP(vector<int> classCnt) { //대의원 반 정원 설정 & 출력
	const int target_sum = 109;
	const int total_iterations = 13;

	uniform_int_distribution<int> uid(6, 13);


	// 109가 될 때까지 반복
	while (true) {
		classCnt.clear();

		// 13번 랜덤 숫자 추출
		for (int i = 0; i < 13; i++) {
			classCnt.push_back(uid(gen));
		}

		// 합 계산
		int sum = 0;
		for (int num : classCnt) {
			sum += num;
		}

		// 합이 109이면 종료
		if (sum == 109) {
			break;
		}
	}

	for (int i = 0; i < classCnt.size(); ++i) {
		cout << i + 1 << "반 - " << classCnt[i] << "명" << " ";
		if (i != classCnt.size() - 1) {
			cout << ", ";
		}
	}

}
