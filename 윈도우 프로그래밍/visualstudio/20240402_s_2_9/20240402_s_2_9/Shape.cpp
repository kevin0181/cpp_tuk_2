class Shape {
public:
    virtual void draw(HDC hdc) = 0; // 도형 그리기를 위한 순수 가상 함수
    virtual ~Shape() {} // 가상 소멸자
};