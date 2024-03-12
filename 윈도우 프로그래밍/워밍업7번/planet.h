#include <string>

using namespace std;

struct planet
{
public:
    string name;
    double size;              // 지구가 반지름 10 기준으로, 행성들의 반지름
    int satellite_cnt;        // 위성의 개수
    double revolution_period; // 공전 주기
    double rotation_cycle;    // 자전 주기
    double idle_speed;        // 공전 속도
    double inclination;       // 기울기
    double dft_sum;           // 태양과의 거리
};