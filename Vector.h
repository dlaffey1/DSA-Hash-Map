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

    // friend std::ostream &operator<<(std::ostream &os, const Vector<T> &v)
    // {
    //     os << "[ ";
    //     for (int i = 0; i < v.getSize(); i++)
    //     {
    //         os << v.array[i];
    //         if (i < v.getSize() - 1)
    //         {
    //             os << ", ";
    //         }
    //     }
    //     os << " ]";
    //     return os;
    // }

    Vector &operator=(const Vector &v);
    T &operator[](int index);
    T &At(int index);
    T &Front();
    T &Back();

    void insert(int index, T element);
    void erase(int index);
    void clear();

private:
    int size;
    int capacity;
    T *array;

    void resize();
};

#endif