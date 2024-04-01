#include <iostream>
#include <string>

using namespace std;

template <typename T>
class List {
public:
	List() : data_(nullptr), size_(0), capacity_(0) {}

	List(T* values, size_t size) :data_(nullptr), size_(0), capacity_(0) {
		reserve(size);
		for (size_t i = 0; i < size; ++i) {
			push_back(values[i]);
		}
	}

	~List(){
		delete[] data_;
	}


	T& operator[](size_t index) {
		return data_[index];
	}

	void push_back(T& value) {
		if (size_ >= capacity_) { //만약 내가 지정한 배열 크기와 용량이 같으면 용량 높임
			reserve(capacity_ * 2 > 0 ? capacity_ * 2 : 0);
		}
		data_[size_++] = value;
	}

	size_t size() {
		return size_;
	}

	size_t capacity() {
		return capacity_;
	}

private:

	T* data_;
	size_t size_;
	size_t capacity_;

	void reserve(size_t new_capacity_) { // 배열의 전체 크기 변화
		T* new_data_ = new T[new_capacity_];

		for (size_t i = 0; i < size_; ++i)
			new_data_[i] = data_[i];

		delete[] data_;

		data_ = new_data_;
		capacity_ = new_capacity_;
	}

};

struct champion {
	string name;
	int hp;
	int mp;
	int speed;
	int range;
	string position;
	bool status;
};

enum order_E {
	SEARCH_E,
	INSERT_E,
	DELETE_E,
	DELETE_ALL_E,
	PRINT_ALL_E,
	FINDMAXHP_E,
	SORTBYHP_E,
	NULL_E
};

order_E order_v(string order);

int main() {

	string order;

	cout << "명령어를 입력하세요 : ";
	cin >> order;

	switch (order_v(order))
	{
	case SEARCH_E:
		break;
	case INSERT_E:
		break;
	case DELETE_E:
		break;
	case DELETE_ALL_E:
		break;
	case PRINT_ALL_E:
		break;
	case FINDMAXHP_E:
		break;
	case SORTBYHP_E:
		break;
	default:
		break;
	}
}



order_E order_v(string order){
	if (order == "Search")
		return SEARCH_E;
	if (order == "Insert")
		return INSERT_E;
	if (order == "Delete")
		return DELETE_E;
	if (order == "DeleteAll")
		return DELETE_ALL_E;
	if (order == "PrintAll")
		return PRINT_ALL_E;
	if (order == "FindMaxHp")
		return FINDMAXHP_E;
	if (order == "SortByHp")
		return SORTBYHP_E;
	return NULL_E;
}