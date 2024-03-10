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
	map<int, vector<int>> candidateClass; //�ĺ� ��
	vector<int> classCnt; // ���ǿ� �� ����
	//map<int, vector<int>> delegateClass; //���ǿ� ��
	vector<int> voteC; // 1000�� ��ǥ

	start1(candidateNum, candidateClass, classCnt, voteC);

}

void start1(int& candidateNum, map<int, vector<int>> candidateClass, vector<int> classCnt, vector<int> voteC) {

	vector<int> candidate;

	cout << "���ǿ� �ĺ��� ���� �Է��ϼ���(150~250) : ";
	cin >> candidateNum;
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ���� ������ �Է��ϴ°� ���� (�Էµ� ���� ����)

	if (cin.fail()) {
		// ���� ������ �ƴմϴ�.
		cin.clear(); // �Է� ���� �����
		cin.ignore(); // ���ۿ� �����ִ� ���ڿ� ����
		start1(candidateNum, candidateClass, classCnt, voteC);
	}

	if (candidateNum > 250 || candidateNum < 150) {
		cout << "�ĺ����� ���� �˸°� �Է����ּ���." << endl;
		start1(candidateNum, candidateClass, classCnt, voteC);
	}

	// create candidate
	for (int i = 0; i < candidateNum; ++i) {
		candidate.push_back(i + 1);
	}

	// push candidate in class
	uniform_int_distribution<int> uid(1, 13);
	for (int i = 0; i < candidateNum; ++i) {
		candidateClass[uid(gen)].push_back(candidate[i]); // �ĺ��ڵ��� ������ �ݿ� �־���.
	}

	// vote ~!
	// push candidate in class
	uniform_int_distribution<int> dis(1, candidateNum);
	for (int i = 0; i < 1000; ++i) {
		voteC.push_back(dis(gen));
	}

	for (const auto& pair : candidateClass) {
		for (size_t i = 0; i < pair.second.size(); ++i) {
			cout << pair.first << "�� - " << pair.second[i] << "��";
			if (i != pair.second.size() - 1) {
				cout << ", ";
			}
		}
		cout << endl;
	}

	while (true) {
		
		cout << endl;
		string orderS{};
		cout << "��ɾ �Է��ϼ��� : ";
		cin >> orderS;

		unordered_map<std::string, int> myMap = {
		{"p", 'p'},
		{"a", 'a'},
		{"b",'b'}
		};

			int value = myMap[orderS];
			switch (value)
			{
			case 'p': //���ǿ� �� ���
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
						cout << "1000 ���Ϸ� �Է����ּ���." << endl;
					break;
				}
				catch (const  std::invalid_argument& e) {
					cerr << "���� �߻�: �Էµ� ���ڿ����� ���ڰ� �ƴ� ���ڰ� ���ԵǾ� �ֽ��ϴ�." << endl;
				}
			}

		cout << endl << "----------------------------------------" << endl << endl;
	}
}
void fB(vector<int> voteC, map<int, vector<int>> candidateClass) {

	for (auto& pair : candidateClass) {
		int key = pair.first; // Ű �б�
		vector<int>& values = pair.second; // ��(����) �б�

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

		cout << key << "�� : ";
		for (const auto& pair : vec) {
			cout << pair.first << "�� - " << pair.second << "ǥ, ";
		}

		cout << endl;

	}

}
void fA(vector<int> voteC, map<int, vector<int>> candidateClass) {

	for (auto& pair : candidateClass) {
		int key = pair.first; // Ű �б�
		vector<int>& values = pair.second; // ��(����) �б�

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

		cout << key << "�� : ";
		for (const auto& pair : vec) {
			cout << pair.first << "�� - " << pair.second << "ǥ, ";
		}

		cout << endl;

	}

}

void fNum(vector<int> voteC, int num, map<int, vector<int>> candidateClass) {

	int voteNum{}; // num�� ��ǥ�� ��� ��ȣ

	for (int i = 0; i < 1000; ++i) {
		if (i + 1 == num) {
			voteNum = voteC[i];
		}
	}

	int classKey{};
	int keyValue{};

	for (const auto& pair : candidateClass) {
		int key = pair.first; // Ű �б�
		const vector<int>& values = pair.second; // ��(����) �б�

		for (int value : values) {
			if (voteNum == value) {
				classKey = key;
				keyValue = value;
			}
		}

	}

	cout << num << "�� ��ǥ�� ����� " << classKey << "�� - " << keyValue << "��" << " �Դϴ�." << endl;
}



void fP(vector<int> classCnt) { //���ǿ� �� ���� ���� & ���
	const int target_sum = 109;
	const int total_iterations = 13;

	uniform_int_distribution<int> uid(6, 13);


	// 109�� �� ������ �ݺ�
	while (true) {
		classCnt.clear();

		// 13�� ���� ���� ����
		for (int i = 0; i < 13; i++) {
			classCnt.push_back(uid(gen));
		}

		// �� ���
		int sum = 0;
		for (int num : classCnt) {
			sum += num;
		}

		// ���� 109�̸� ����
		if (sum == 109) {
			break;
		}
	}

	for (int i = 0; i < classCnt.size(); ++i) {
		cout << i + 1 << "�� - " << classCnt[i] << "��" << " ";
		if (i != classCnt.size() - 1) {
			cout << ", ";
		}
	}

}
