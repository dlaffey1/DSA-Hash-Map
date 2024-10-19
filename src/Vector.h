// Followed a tutorial on YouTube that implemented an int Vector class: https://www.youtube.com/watch?v=YpNCBw-cPWw
// Then turned it into a template class.

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

template <typename T>
class Vector
{
public:
    Vector();
    Vector(const Vector &v);
    Vector(int elements, T value = T());

    ~Vector();

    void push_back(T element);
    void pop_back();

    bool empty() const;
    int getSize() const;
    int getCapacity() const;

    bool operator==(const Vector &v) const;

    friend std::ostream &operator<<(std::ostream &os, const Vector<T> &v)
    {
        for (int i = 0; i < v.size; i++)
        {
            os << v.array[i] << " ";
        }

        os << " || ";

        for (int i = v.size; i < v.capacity; i++)
        {
            os << v.array[i] << " ";
        }

        os << std::endl;

        return os;
    }

    Vector &operator=(const Vector &v);
    T &operator[](int index);
    const T &operator[](int index) const;
    T &At(int index);
    const T &At(int index) const;
    T &Front();
    const T &Front() const;
    T &Back();
    const T &Back() const;

    void insert(int index, T element);
    void erase(int index);
    void clear();
    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;
    void resize(int newSize);
private:
    int size;
    int capacity;
    T *array;

    void resize();
};

#endif