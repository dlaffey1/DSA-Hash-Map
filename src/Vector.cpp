#include "Vector.h"
#include "hash_map.h"

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
        array = new T[capacity];  // Allocate new array of the correct type
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
    if (index < 0 || size <= index)
    {
        throw std::out_of_range("Index out of bounds!");
    }
    return array[index];
}

template <typename T>
const T &Vector<T>::operator[](int index) const
{
    if (index < 0 || size <= index)
    {
        throw std::out_of_range("Index out of bounds!");
    }
    return array[index];
}

template <typename T>
T &Vector<T>::At(int index)
{
    if (index < 0 || size <= index)
    {
        throw std::out_of_range("Index out of bounds!");
    }

    std::cout << "Returning element at index: " << index << std::endl;
    std::cout << "Element: " << array[index] << std::endl;
    return array[index];
}

template <typename T>
const T &Vector<T>::At(int index) const
{
    if (index < 0 || size <= index)
    {
        throw std::out_of_range("Index out of bounds!");
    }
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
const T &Vector<T>::Front() const
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
const T &Vector<T>::Back() const
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
    std::cout << "Inserting element: " << element << " at index: " << index << std::endl;
    if (index < 0 || index > size)
    {
        throw std::out_of_range("Index out of bounds!");
    }

    if (size == capacity)
    {
        resize();
    }
    for (int i = size - 1; i > index; --i)
    {
        array[i] = array[i - 1];
    }
    array[index] = element;
    ++size;
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
void Vector<T>::resize(int newSize)
{
    if (newSize > capacity)
    {
        T* newArray = new T[newSize];
        for (int i = 0; i < size; ++i)
        {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newSize;
    }
    size = newSize;
}
template <typename T>
void Vector<T>::resize()
{
    int new_capacity = capacity == 0 ? 1 : capacity * 2;
    T* temp = new T[new_capacity];
    for (int i = 0; i < size; ++i)
    {
        temp[i] = array[i];
    }
    for (int i = size; i < new_capacity; ++i)
    {
        temp[i] = T();
    }

    delete[] array;
    array = temp;
    capacity = new_capacity;
}


template <typename T>

T* Vector<T>::begin() {

    return array;  // Points to the first element of the array

}



template <typename T>

T* Vector<T>::end() {

    return array + size;  // Points one past the last element

}



template <typename T>

const T* Vector<T>::begin() const {

    return array;

}



template <typename T>

const T* Vector<T>::end() const {

    return array + size;

}


template class Vector<std::string>;
template class Vector<WordEntry>;