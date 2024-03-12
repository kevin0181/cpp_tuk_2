#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include "planet.h"

using namespace std;

void printPlanet(vector<planet> planetList);
void aF(vector<planet> planetList);
void bF(vector<planet> planetList);
void cF(vector<planet> planetList);
void dF(vector<planet> planetList);
void eF(vector<planet> planetList);

int main()
{
    vector<planet> planetList = {
        {"태양", 1090, 0, 0, 25.38, 0, 7.25, 0.0},
        {"수성", 3.8, 0, 87.97, 58.6462, 47.8725, 0.01, 0.4},
        {"금성", 9.5, 0, 224.7, -243.0185, 35.0214, 2.64, 0.7},
        {"지구", 10, 1, 365.26, 0.99726963, 29.7859, 23.44, 1.0},
        {"화성", 5.3, 2, 686.96, 1.02595676, 24.1309, 25.19, 1.5},
        {"목성", 112, 79, 4333.29, 0.41354, 13.0697, 3.12, 5.2},
        {"토성", 94.5, 82, 10756.20, 0.44401, 9.6724, 26.73, 9.5},
        {"천왕성", 40, 27, 30707.49, 0.71833, 6.8352, 82.23, 19.2},
        {"해왕성", 38.8, 14, 60223.35, 0.67125, 5.4778, 28.33, 30.1}};

    while (true)
    {
        char orderC{};
        cout << "명령어를 입력하세요 : ";
        cin >> orderC;
        switch (orderC)
        {
        case 'q':
            exit(0);
            break;
        case 'p':
            printPlanet(planetList);
            break;
        case 'a':
            aF(planetList);
            break;
        case 'b':
            bF(planetList);
            break;
        case 'c':
            cF(planetList);
            break;
        case 'd':
            dF(planetList);
            break;
        case 'e':
            eF(planetList);
            break;
        default:
            break;
        }
    }
}
void eF(vector<planet> planetList)
{
    vector<planet> getPlanetList = planetList;
    sort(getPlanetList.begin(), getPlanetList.end(), [](const planet &a, const planet &b)
         { return a.inclination < b.inclination; });
    printPlanet(getPlanetList);
}

void dF(vector<planet> planetList)
{
    vector<planet> getPlanetList = planetList;
    for (planet &p : getPlanetList)
    {
        p.rotation_cycle = p.rotation_cycle / 24;
    }
    printPlanet(getPlanetList);
}

void cF(vector<planet> planetList)
{
    vector<planet> getPlanetList = planetList;
    sort(getPlanetList.begin(), getPlanetList.end(), [](const planet &a, const planet &b)
         { return a.dft_sum < b.dft_sum; });
    printPlanet(getPlanetList);
}

void bF(vector<planet> planetList)
{
    vector<planet> getPlanetList = planetList;
    sort(getPlanetList.begin(), getPlanetList.end(), [](const planet &a, const planet &b)
         { return a.size > b.size; });
    printPlanet(getPlanetList);
}

void aF(vector<planet> planetList)
{
    vector<planet> getPlanetList = planetList;

    sort(getPlanetList.begin(), getPlanetList.end(), [](const planet &a, const planet &b)
         { return a.idle_speed < b.idle_speed; });

    printPlanet(getPlanetList);
}

void printPlanet(vector<planet> planetList)
{
    // 각 열의 너비 설정
    const int nameWidth = 12;
    const int sizeWidth = 8;
    const int satelliteWidth = 14;
    const int revolutionWidth = 14;
    const int rotationWidth = 14;
    const int speedWidth = 14;
    const int inclinationWidth = 14;
    const int distanceWidth = 14;

    // 열 제목 출력
    cout << setw(nameWidth) << left << "이름"
         << setw(sizeWidth) << "크기"
         << setw(satelliteWidth) << "위성의 개수  "
         << setw(revolutionWidth) << "공전 주기"
         << setw(rotationWidth) << "자전 주기"
         << setw(speedWidth) << "공전 속도"
         << setw(inclinationWidth) << "기울기"
         << setw(distanceWidth) << "태양과의 거리" << endl;

    // 각 행 출력
    for (const planet &p : planetList)
    {
        cout << setw(nameWidth) << left << p.name
             << setw(sizeWidth) << p.size
             << setw(satelliteWidth) << p.satellite_cnt
             << setw(revolutionWidth) << p.revolution_period
             << setw(rotationWidth) << p.rotation_cycle
             << setw(speedWidth) << p.idle_speed
             << setw(inclinationWidth) << p.inclination
             << setw(distanceWidth) << p.dft_sum << endl;
    }
}