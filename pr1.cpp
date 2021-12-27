#include <iostream>
#include <vector>
#include <algorithm>

template<class T>
class HeapOverArray {
    std::vector<T> v;

public:
    HeapOverArray() {}
    HeapOverArray(const std::vector<T>& initv) : v(initv) {}

    const std::vector<T>& getVector() const { return v; }
    bool addNode(const T& o) {
        size_t pos = v.size();
        if (pos == 0) {
            v.push_back(o);
            return true;
        }
        size_t parent_pos = (pos - 1) / 2;
        v.push_back(o);
        while (pos != 0 && o > v[parent_pos]) {
            std::swap(v[pos], v[parent_pos]);
            pos = parent_pos;
            parent_pos = (pos - 1) / 2;
        }
        if (pos == 0) return true; // мы в корне
        if (pos == v.size() - 1)  return true; // даже если мы слева, то справа ничего нет
        if (pos % 2 == 1) return true; // мы справа
        // осталось что мы слева и не в конце, значит право есть
        if (v[pos] > v[pos + 1]) {
            //swap(v[pos, v[pos + 1]]);
            auto temp = v[pos];
            v[pos] = v[pos + 1];
            v[pos + 1] = temp;
        }
        return true;
    }


    bool removeNode(const T& o) {
        size_t pos = 0;
        if (v.size() == 0) // пустое дерево
            return false;
        while (pos < v.size() && v[pos] != o )
            ++pos;
        if (pos = v.size())             //не нашли элемент
            return false;
        v.pop_back();
        if (pos == v.size() - 1) {      //удаляем последний
            return true;
        }
        size_t parent_pos = (pos - 1) / 2;
        v[i] = o;
        while (v[parent_pos] < v[pos]) {    //поднимаемся по дереву
            swap(v[pos], v[parent_pos]);
            pos = parent_pos;
            parent_pos= (pos - 1) / 2;
        }
        if (pos == 0)
            return true;
        while (2*pos < v.size() && (v[2 * pos] > o || v[2*pos+1] > o )) { //спускаемся вниз
            if (v[2 * pos] > o && v[2 * pos + 1] < o) {// больше только левый
                swap(v[2 * pos], v[pos]);
                pos = pos * 2;
            }
            else if (v[2 * pos] > o && v[2 * pos + 1] > o) { // оба больше
                if (v[2 * pos + 1] > v[2 * pos]) {
                    swap(v[2 * pos + 1], v[pos]);
                    pos = 2 * pos + 1;
                }
                else {
                    swap(v[pos], v[2 * pos]);
                    pos = 2 * pos;
                }
            }
            else {  //больше тольео правый
                swap(v[2 * pos + 1], v[pos]);
                pos = 2 * pos + 1;
            }
        }
        return true;

    }

    class iterator {
        std::vector<T>* v;
        size_t idx;

    public:
        iterator() : v(nullptr) {}
        iterator(std::vector<T>* v_, size_t idx_) : v(v_), idx(idx_) {}

        size_t findLeftmost(size_t pos) {
            while (pos * 2 <= v->size()) {
                pos *= 2;
            }
            return pos;
        }

        size_t findFirstEvenAncestor(size_t pos) {
            while (pos % 2 == 1 && pos != 1) {
                pos /= 2;
            }
            return pos;
        }

        bool hasRight(size_t pos) {
            return (pos * 2 + 1 <= v->size());
        }

        iterator& operator++() {
            size_t pos = idx + 1;
            if (pos % 2 == 1) {
                if (hasRight(pos)) {
                    pos = pos * 2 + 1;
                    pos = findLeftmost(pos);
                }
                else {
                    pos = findFirstEvenAncestor(pos);
                    if (pos == 1) {
                        pos = v->size() + 1;
                        v = nullptr;
                    }
                    else {
                        pos /= 2;
                    }
                }
            }
            else {
                if (hasRight(pos)) {
                    pos = pos * 2 + 1;
                    pos = findLeftmost(pos);
                }
                else {
                    pos /= 2;
                }
            }
            idx = pos - 1;
            return *this;
        }

        iterator operator++(int) {
            auto prev = *this;
            ++(*this);
            return prev;
        }

        operator bool() const { return v != nullptr; }

        const T& operator*() const { return (*v)[idx]; }
        T& operator*() { return (*v)[idx]; }
    };

    iterator begin() {
        if (v.empty()) {
            return iterator();
        }
        size_t idx = 0;
        while (idx * 2 < v.size()) {
            idx = idx * 2 + 1;
        }
        return iterator(&v, idx);
    }

    iterator end() {
        return iterator();
    }
};

template<class T>
bool testHeapAdd(const std::vector<T>& initial, const T& value,
    const std::vector<T>& expected)
{
    HeapOverArray<T> heap(initial);
    heap.addNode(value);
    auto v = heap.getVector();
    if (v.size() != expected.size()) {
        std::cerr << "size difference: expected " << expected.size() <<
            ", got " << v.size() << std::endl;
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++) {
        if (v[i] != expected[i]) {
            std::cerr << "diffecrence in " << i << ": expected " <<
                expected[i] << ", got " << v[i] << std::endl;
            return false;
        }
    }
    return true;
}

template<class T>
bool testHeapIterator(const std::vector<T>& initial,
    const std::vector<T>& expected)
{
    HeapOverArray<T> heap(initial);
    std::vector<T> v;
    for (auto it = heap.begin(); it; it++) {
        v.push_back(*it);
    }
    if (v.size() != expected.size()) {
        std::cerr << "size difference: expected " << expected.size() <<
            ", got " << v.size() << std::endl;
        return false;
    }
    for (size_t i = 0; i < expected.size(); i++) {
        if (v[i] != expected[i]) {
            std::cerr << "diffecrence in " << i << ": expected " <<
                expected[i] << ", got " << v[i] << std::endl;
            return false;
        }
    }
    return true;
}
int main() {
    std::vector<int> initial = { 7, 3, 4 };
    std::vector<int> expected = { 8, 7, 4, 3 };
    testHeapAdd(initial, 8, expected);

    initial = { 100, 70, 60, 25, 10, 40, 5, 8, 2, 9, 1, 3 };
    expected = { 8, 25, 2, 70, 9, 10, 1, 100, 3, 40, 60, 5 };
    testHeapIterator(initial, expected);
    return 0;
}