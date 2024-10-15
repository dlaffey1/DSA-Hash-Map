#include "vector.h"

// Constructor
template <typename T>
Vector<T>::Vector()
    : size(0), capacity(10), array(new T[capacity]())
{
    // nothing here...
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector &v)
    : size(v.size), capacity(v.capacity), array(new T[v.capacity])
{
    for (int i = 0; i < size; ++i)
    {
        array[i] = v.array[i];
    }
}

// Constructor with parameters
template <typename T>
Vector<T>::Vector(int elements, T value)
    : size(elements), capacity(elements), array(new T[capacity])
{
    for (int i = 0; i < size; i++)
    {
        array[i] = value;
    }
}

// Destructor
template <typename T>
Vector<T>::~Vector()
{
    delete[] array;
}

template <typename T>
void Vector<T>::push_back(T element)
{
    if (size == capacity)
    {
        resize();
    }
    array[size++] = element;
}

template <typename T>
void Vector<T>::pop_back()
{
    if (size == 0)
    {
        throw std::out_of_range("Pop back on empty vector!");
    }
    --size;
    array[size] = T();
}

template <typename T>
bool Vector<T>::empty() const
{
    return size == 0;
}

template <typename T>
int Vector<T>::getSize() const
{
    return size;
}

template <typename T>
int Vector<T>::getCapacity() const
{
    return capacity;
}

template <typename T>
bool Vector<T>::operator==(const Vector &v) const
{
    if (size != v.size)
    {
        return false;
    }

    for (int i = 0; i < size; ++i)
    {
        if (array[i] != v.array[i])
        {
            return false;
        }
    }

    return true;
}

template <typename T>
Vector<T> &Vector<T>::operator=(const Vector &v)
{
    if (v.size > size)
    {
        delete[] array;
        capacity = v.size + 5;
        array = new int[capacity];
    }
    for (int i = 0; i < v.size; ++i)
    {
        array[i] = v.array[i];
    }

    size = v.size;
    return *this;
}

template <typename T>
T &Vector<T>::operator[](int index)
{
    return array[index];
}

template <typename T>
T &Vector<T>::At(int index)
{
    if (index < 0 || index >= size)
    {
        throw std::out_of_range("Index out of bounds!");
    }

    std::cout << "Returning element at index: " << index << std::endl;
    std::cout << "Element: " << array[index] << std::endl;
    return array[index];
}

template <typename T>
T &Vector<T>::Front()
{
    if (size == 0)
    {
        throw std::out_of_range("Vector is empty!");
    }
    return array[0];
}

template <typename T>
T &Vector<T>::Back()
{
    if (size == 0)
    {
        throw std::out_of_range("Vector is empty!");
    }
    return array[size - 1];
}

template <typename T>
void Vector<T>::insert(int index, T element)
{
    // std::cout << "Inserting element: " << element << " at index: " << index << std::endl;
    if (index < 0 || index > size)
    {
        throw std::out_of_range("Index out of bounds!");
    }

    if (size != capacity)
    {
        for (int i = size - 1; i >= index; --i)
        {
            array[i + 1] = array[i];
        }
        array[index] = element;
        ++size;
    }
    else
    {
        resize();
        insert(index, element);
    }
}

template <typename T>
void Vector<T>::erase(int index)
{
    if (index < 0 || index >= size)
    {
        throw std::out_of_range("Index out of bounds!");
    }

    for (int i = index; i < size - 1; ++i)
    {
        array[i] = array[i + 1];
    }

    --size;
}

template <typename T>
void Vector<T>::clear()
{
    size = 0;
}

template <typename T>
void Vector<T>::resize()
{
    int new_capacity = capacity == 0 ? 1 : capacity * 2;
    T* temp = new T[new_capacity]();
    for (int i = 0; i < size; ++i)
    {
        temp[i] = array[i];
    }

    delete[] array;
    array = temp;
}