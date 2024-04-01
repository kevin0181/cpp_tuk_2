#include <iostream>
#include <string>

using namespace std;

template <typename T>
class CList
{
public:
    CList() : data_(nullptr), size_(0), capacity_(0) {}

    CList(T *values, int size) : data_(nullptr), size_(0), capacity_(0)
    {
        reserve(size);
        for (int i = 0; i < size; ++i)
            push_back(values[i]);
    }

    ~CList()
    {
        delete[] data_;
    }

    void push_back(const T &value)
    {
        if (size_ >= capacity_)
        {
            reserve(capacity_ * 2 > 0 ? capacity_ * 2 : 1);
        }

        data_[size_++] = value;
    }

    T &operator[](int index)
    {
        return data_[index];
    }

    size_t size()
    {
        return size_;
    }

    size_t capacity()
    {
        return capacity_;
    }

    void erase(size_t index)
    {
        if (index < size_)
        {
            for (size_t i = index; i < size_ - 1; ++i)
            {
                data_[i] = data_[i + 1];
            }
            --size_;
        }
    }

private:
    T *data_;
    size_t size_;     // 크기
    size_t capacity_; // 용량

    void reserve(size_t new_capacity)
    {
        T *new_data_ = new T[new_capacity];

        for (size_t i = 0; i < size_; ++i)
        {
            new_data_[i] = data_[i];
        }

        delete[] data_;

        data_ = new_data_;
        capacity_ = new_capacity;
    }
};

struct Champion
{
    string name;
    int hp;
    int mp;
    int speed;
    int range;
    string position;
};

int main()
{
    Champion default_champion[] = {
        {"Jayce", 3466, 8307, 452, 73, "mid"},
        {"Vayne", 3318, 2790, 390, 60, "support"},
        {"Lee Sin", 9864, 6257, 747, 19, "support"},
        {"Teemo", 1915, 1313, 700, 68, "mid"},
        {"Nami", 2968, 2153, 803, 74, "support"},
        {"Jinx", 2583, 3042, 123, 53, "jungle"},
        {"Cassiopeia", 6067, 7330, 683, 24, "top"},
        {"Ahri", 4573, 7079, 177, 79, "top"},
        {"Fiora", 2591, 9329, 337, 81, "jungle"},
        {"Zac", 9864, 7272, 405, 49, "bottom"},
        {"Janna", 4922, 2275, 690, 66, "jungle"},
        {"Ashe", 7421, 5549, 428, 31, "top"},
        {"Twitch", 2980, 7703, 204, 91, "mid"},
        {"Ezreal", 3660, 9835, 274, 44, "mid"},
        {"Sona", 5655, 8442, 252, 26, "bottom"},
        {"Lulu", 3231, 2014, 503, 32, "support"},
        {"Amumu", 6763, 3994, 427, 19, "jungle"},
        {"Master Yi", 5737, 7168, 434, 61, "top"},
        {"Jarvan", 1895, 3070, 323, 80, "mid"},
        {"Shen", 4794, 9220, 783, 42, "mid"},
        {"Fizz", 9864, 1913, 433, 38, "bottom"},
        {"Taric", 6158, 7451, 160, 36, "bottom"}};

    size_t ch_size = sizeof(default_champion) / sizeof(default_champion[0]);

    CList champion_list(default_champion, ch_size);

    Champion ch1 = {
        "Yasuo",
        1111,
        3333,
        4444,
        5555,
        "mid"};

    champion_list.push_back(ch1);

    for (int i = 0; i < champion_list.size(); ++i)
    {
        if (champion_list[i].name == "Zac")
            champion_list.erase(i);
    }

    for (int i = champion_list.size(); i > 0; --i)
    {
        if (champion_list[i].position == "support")
            champion_list.erase(i);
    }

    for (size_t i = 0; i < champion_list.size(); ++i)
    {
        cout << champion_list[i].name << ", " << champion_list[i].position << endl;
    }
}