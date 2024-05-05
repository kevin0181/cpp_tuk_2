#include <iostream>
#include <string>

using namespace std;

template <typename T>
class List
{

public:
    List() : data_(nullptr), size_(0), capacitiy_(0) {}

    ~List()
    {
        delete[] data_;
    }

    List(T *values, size_t size)
    {
        reserve(size);
        for (size_t i = 0; i < size; ++i)
        {
            push_back(values[i]);
        }
    }

    void push_back(T &value)
    {
        if (size_ >= capacitiy_)
        {
            reserve(capacitiy_ * 2 > 0 ? capacitiy_ * 2 : 1);
        }

        data_[size_++] = value;
    }

private:
    T *data_;
    size_t size_;
    size_t capacitiy_;

    void reserve(size_t new_capacitiy_)
    {
        T *new_data_ = new T[new_capacitiy_];

        for (size_t i = 0; i < size_; ++i)
        {
            new_data_[i] = data_[i];
        }

        delete[] data_;

        data_ = new_data_;
        capacitiy_ = new_capacitiy_;
    }
};

int main()
{
}