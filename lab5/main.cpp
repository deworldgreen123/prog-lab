
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>


template<typename T>
class CircularBuffer;

template<typename T>
class iterator;

template<typename T>
class const_iterator;

template<typename T>
class baseIterator : std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, T*, T&> {
    friend class CircularBuffer<T>;
    friend class const_iterator<T>;
    friend class iterator<T>;

private:
    explicit baseIterator(T* buff1, T* begin1, T* end1):
            buff(buff1),
            begin(begin1),
            end(end1) {}

    T* begin;
    T* end;
    T* buff;
public:
    baseIterator& operator=(const baseIterator& other) {
        this->buff = other.buff;
        this->begin = other.begin;
        this->end = other.end;
        return *this;
    }
    virtual baseIterator& operator++() {
        ++buff;
        if (buff == end) {
            buff = begin;
        }
        return *this;
    }

    virtual baseIterator& operator++(T) { //++it
        baseIterator other = *this;
        ++*this;
        return other;
    }

    virtual baseIterator& operator--() {
        if (buff == begin) {
            buff = end - 1;
        }
        --buff;
        return *this;
    }

    virtual baseIterator& operator--(T) {
        baseIterator other = *this;
        --*this;
        return other;
    }

    bool operator==(const baseIterator& other) const {
        return buff == other.buff;
    }
    bool operator!=(const baseIterator& other) const {
        return !(*this == other);
    }

    bool operator<(const baseIterator& other) const {
        return buff < other.buff;
    }
    bool operator>(const baseIterator& other) const {
        return !(*this < other);
    }
    bool operator<=(const baseIterator& other) const {
        return *this < other || *this == other;
    }
    bool operator>=(const baseIterator& other) const {
        return *this > other || *this == other;
    }

    ptrdiff_t operator-(baseIterator const& other) const {
        if (*this < other)
            return (buff - begin + end - other.buff);
        return buff - other.buff;
    }

    baseIterator &operator+=(ptrdiff_t n) {
        n %= (end - begin);
        if (buff - begin + n < end - begin)
            buff = buff + n;
        else
            buff = buff - (end - begin - n);
        return *this;
    }
    baseIterator &operator-=(ptrdiff_t n) {
        n %= (end - begin);
        if (buff - begin + n < end - begin)
            buff = buff + (end - begin - n);
        else
            buff = buff - n;
        return *this;
    }

    baseIterator operator+(ptrdiff_t n) {
        baseIterator<T> res = *this;
        res += n;
        return res;
    }
    baseIterator operator-(ptrdiff_t n) {
        baseIterator<T> res = *this;
        res -= n;
        return res;
    }
};

template<typename T>
class iterator : public baseIterator<T>{
    friend class CircularBuffer<T>;
private:
    explicit iterator(T* buff, T* begin, T* end):baseIterator<T>(buff, begin, end) {
    }

public:
    iterator& operator=(const iterator& other) {
        this->buff = other.buff;
        this->begin = other.begin;
        this->end = other.end;
        return *this;
    }
    ~iterator() = default;

    T& operator*() const {
        return *this->buff;
    }

    /*iterator& operator++() override{
        ++*this;
        return *this;
    }

    iterator& operator++(T) override{
        iterator other = *this;
        ++*this;
        return other;
    }

    iterator& operator--() override{
        --*this;
        return *this;
    }

    iterator& operator--(T) override{
        iterator other = *this;
        --*this;
        return other;
    }*/
};

template<typename T>
class [[maybe_unused]] const_iterator : public baseIterator<T>{
    friend class CircularBuffer<T>;

private:
    explicit const_iterator(T* buff, T* begin, T* end):baseIterator<T>(buff, begin, end) {
    }

public:
    const_iterator& operator=(const const_iterator& other) {
        this->buff = other.buff;
        this->begin = other.begin;
        this->end = other.end;
        return *this;
    }

    ~const_iterator() = default;

    const T& operator*() const {
        return *this->buff;
    }


};
template<typename T>
baseIterator<T> operator+(ptrdiff_t n, baseIterator<T> it) {
    return it + n;
}
template<typename T>
iterator<T> operator+(ptrdiff_t n, iterator<T> it) {
    return it + n;
}
template<typename T>
const_iterator<T> operator+(ptrdiff_t n, const_iterator<T> it) {
    return it + n;
}

template<typename T>
class CircularBuffer{
private:
    size_t size;
    size_t capacity;
    T* data;
    size_t first;

public:
    explicit CircularBuffer(size_t len = 0):
            size(0),
            capacity(len),
            first(0),
            data(new T[len + 1])
            {}

    ~CircularBuffer() {
        delete[] data;
    }

    CircularBuffer& operator=(const CircularBuffer& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        first = other.first;
        data = new T[capacity + 1];
        for (int i = 0; i < capacity; ++i) {
            data[i] = other.data[i];
        }
        return *this;
    }

    iterator<T> begin(){
        return iterator<T>(data + first, data, data + capacity + 1);
    }

    iterator<T> end(){
        return iterator<T>(data + (first + size) % (capacity + 1), data, data + capacity + 1);
    }

    [[maybe_unused]] const_iterator<T> cbegin(){
        return const_iterator<T>(data + first, data, data + capacity + 1);
    }

    [[maybe_unused]] const_iterator<T> cend(){
        return const_iterator<T>(data + (first + size) % (capacity + 1), data, data + capacity + 1);
    }

    T operator[](size_t i) const {
        return data[(first + i) % (capacity + 1)];
    }

    T &operator[](size_t i) {
        return data[(first + i) % (capacity + 1)];
    }

    size_t len(){
        return size;
    }

    [[maybe_unused]] void resize(size_t len){
        T *new_arr = new T [len + 1];
        size_t s = capacity < len ? capacity : len;
        capacity = len;

        for (size_t i = 0; i < s; ++i)
            new_arr[i] = data[i];

        delete [] data;

        data = new_arr;
    }

    [[maybe_unused]] void resize(size_t len, T a){
        T *new_arr = new T [len + 1];
        delete [] data;
        for (auto i : new_arr)
            i = a;
        data = new_arr;
    }

    void push_back(T a){
        if (size == 0) {
            data[0] = a;
        } else {
            data[(first + size) % (capacity + 1)] = a;
        }
        if (size != capacity) {
            ++size;
        } else {
            first = (first + 1) % (capacity + 1);
        }
    };

    void pop_back(){
        if (size == 1) {
            first = 0;
        }
        size--;
    }

    [[maybe_unused]] void push_front(T a){
        if (size == 0) {
            data[0] = a;
        } else {
            first = (first + capacity) % (capacity + 1);
            data[first] = a;
        }
        if (size != capacity) {
            ++size;
        }
    };

    void pop_front(){
        if (size == 1) {
            first = 0;
            size--;
            return;
        }
        first++;
        size--;
        first %= (capacity + 1);
    }

    void print(){
        for (int i = 0; i <= capacity; i++) {
            std::cout << *(data + i) << " ";
        }
        std::cout << "\n" << first << size << "\n";
    }
};

int main() {
    CircularBuffer<int> ddd(10);
    for (int i = 0; i < 120; i++){
        ddd.push_back(i);
    }
    for (auto it = ddd.begin(); it != ddd.end(); ++it){
        std::cout << *it << (it != ddd.end() - 1 ? " | " : "\n");
    }
    std::cout << ddd.len() << "\n";
    for (auto it = ddd.begin(); it != ddd.end(); ++it){
        std::cout << *it << (it != ddd.end() - 1 ? " | " : "");
    }
    return 0;
}
