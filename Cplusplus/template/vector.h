#ifndef VECTOR_H
#define VECTOR_H
template <class T>
namespace Owner {
    class vector
    {
    public:
        vector();
        vector(int size);
        vector &operator = (const vector &vr);
        vector(const vector &vr);
        int size();
        int capacity();
        T pop_back();
        void push_back(T & item);
        T * begin();
        T * end();
        T * rbegin();
        T * rend;
        bool insert();
        bool insert();
        bool clear();
        bool erase();
        bool empty();
        T front();
        T back();
    private:
        int _size;
        int _capacity;
        T *Tvector;
        void expand();
        void copyfrom();
    };
}

#endif // VECTOR_H
