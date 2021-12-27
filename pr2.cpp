#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

template< class T>
class HeapOverArray {
protected: vector<T> v;
public:
    HeapOverArray() { }
    HeapOverArray (const vector<T> &initv) : v(initv) {}

    const vector<T> getVector() const { return v; }

    bool addNode(const T& o) {




        size_t  pos = v.size();
        if (pos == 0) {
            v.push_back(o);
            return true;
        }

        for (auto i = 0; i < pos; ++i) {
            if (v[i] == o)
                return false;
        }
        v.push_back(o);
        size_t parent_pos = (pos + 1) / 2 - 1;

        while (parent_pos >= 0 && v[pos] > v[parent_pos])
        {
            //swap(v[pos], v[parent_pos]);
            auto temp = v[pos];
            v[pos] = v[parent_pos];
            v[parent_pos] = temp;

            pos = parent_pos;
            parent_pos = (pos + 1) / 2 - 1;
        }

        if (pos == 0) return true; // мы в корне
        if (pos == v.size() - 1)  return true; // даже если мы слева, то справа ничего нет
        if (pos % 2 == 1) return true; // мы справа
        // осталось что мы слева и не в конце, значит право есть
        if (v[pos] > v[pos + 1]) {
            //swap(v[pos, v[pos + 1]]);
            auto temp = v[pos];
            v[pos] = v[pos+1];
            v[pos+1] = temp;
        }
        return true;
        
        return true;

    }

    bool removeNode(const T& o) {

    }

    ~HeapOverArray() {}
};

template <class T>
bool testHeapAdd(const vector<T>& initial, const T& value, const vector<T>& expected) {
    HeapOverArray<T> heap(initial);
    heap.addNode(value);
    auto v = heap.getVector();
    if (v.size() != expected.size()) {
        cerr << "size difference: expected " << expected.size() << ", got " << v.size() << endl;
        return false;
    }
    for(size_t i=0; i<expected.size(); ++i)
        if (v[i] != expected[i]) {
            cerr << "difference in " << i << ": expected " << expected[i] << ", got " << v[i] << endl;
            return false;
        }

    return true;
}

int main()
{
    vector<int> initial = { 7,3,4 };
    vector<int> expected = { 8,7,3,4 };
    testHeapAdd(initial, 8, expected);
    return 0;

}

