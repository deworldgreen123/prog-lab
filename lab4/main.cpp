#include <iostream>
#include <vector>
#include <list>
#include <iterator>

using namespace std;

bool mod2(int value){
    return value % 2 == 0;
}

bool sort(int x, int y){
    return x >= y;
}

template <typename Init, typename Pred>
constexpr bool all_of(Init _begin, Init _end, Pred _pred){
    for (; _begin != _end; _begin++){
        if (!_pred(*_begin))
            return false;
    }
    return true;
}

template <typename Init, typename Pred>
constexpr bool any_of(Init _begin, Init _end, Pred _pred){
    for (; _begin != _end; _begin++){
        if (_pred(*_begin))
            return true;
    }
    return false;
}

template <typename Init, typename Pred>
constexpr bool none_of(Init _begin, Init _end, Pred _pred){
    for (; _begin != _end; _begin++){
        if (_pred(*_begin))
            return false;
    }
    return true;
}

template <typename Init, typename Pred>
constexpr bool one_of(Init _begin, Init _end, Pred _pred){
    int one = 0;
    for (; _begin != _end && one <= 1; _begin++){
        if (_pred(*_begin))
            one++;
    }
    return one == 1;
}

template <typename Init, typename Pred>
constexpr bool is_sorted(Init _begin, Init _end, Pred _pred){
    if (_begin == _end)
        return true;
    Init k = _begin;
    k++;
    for (; k != _end; k++, _begin++){
        if (!_pred(*_begin, * (k)))
            return false;
    }
    return true;
}

template <typename Init, typename Pred>
constexpr bool is_partitioned(Init _begin, Init _end, Pred _pred){
    if (_begin == _end)
        return true;
    bool check = _pred(*_begin++);

    for (; _begin != _end; _begin++)
        if (check != _pred(*_begin))
            break;

    for (; _begin != _end; _begin++)
        if (check == _pred(*_begin))
            return false;

    return true;
}

template <typename Init, typename Value>
constexpr Value find_not(Init _begin, Init _end, Value _value){
    for (; _begin != _end; _begin++)
        if (_value != *_begin)
            return *_begin;
    return _value;
}

template <typename Init, typename Value>
constexpr Value find_backward(Init _begin, Init _end, Value _value){
    for (; _end != _begin; _end--)
        if (_value == *_end)
            return *_end;
    return _value;
}

template <typename Init, typename Pred>
constexpr bool is_palindrome(Init _begin, Init _end, Pred _pred){
    if (_begin == _end)
        return true;
    auto it = _begin;
    _end--;
    while (_end != _begin) {
        if(_pred(*it) != _pred(*_end))
            return false;
        _end--;
        it++;
    }
    return true;
}

int main() {
    vector<int> A{4, 3, 2};
    int B[3] = {2, 2, 2};
    cout << all_of(begin(B), end(B), mod2) << " ";
    cout << is_sorted(begin(B), end(B), sort) << " ";
}
