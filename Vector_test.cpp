#include <iostream>
#include <stdexcept>
#include <cassert>
#include "Vector.h"
#include "Vector.cpp"
#include <string>

void test_push_back();
void test_pop_back();
void test_insert();
void test_erase();
void test_accessing_elems();
void test_clear();
void test_exceptions();

int main()
{
    test_push_back();
    test_pop_back();
    test_insert();
    test_erase();
    // test_accessing_elems();
    // test_clear();
    // test_exceptions();

    // Vector<int> v;
    // v.push_back(1);
    // v.push_back(2);
    // v.push_back(3);
    // v.push_back(4);
    // v.push_back(5);
    // v.push_back(6);
    // std::cout << v.getSize() << std::endl;
    // v.pop_back();
    // v.pop_back();
    // std::cout << v.getSize() << std::endl;

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// Test adding elements to the end of the vector using push_back()
void test_push_back()
{
    Vector<int> v;
    assert(v.empty() == true);

    v.push_back(1);
    assert(v.empty() == false);
    assert(v.getSize() == 1);
    assert(v.getCapacity() > v.getSize());
    assert(v.Front() == 1);

    v.push_back(2);
    v.push_back(3);
    assert(v.getSize() == 3);
    assert(v.Back() == 3);

    std::cout << "test_push_back(): PASSED" << std::endl;
}

// Test removing elements from the end of the vector using pop_back()
void test_pop_back()
{
    Vector<std::string> v;
    v.push_back("1");
    v.push_back("2");
    v.push_back("3");
    assert(v.getSize() == 3);

    v.pop_back();
    assert(v.getSize() == 2);
    assert(v.Back() == "2");

    v.pop_back();
    v.pop_back();
    assert(v.empty() == true);

    std::cout << "test_pop_back(): PASSED" << std::endl;
}

// Test inserting elements at a specific index
void test_insert()
{
    Vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    v.push_back('d');

    v.insert(4, 'e');
    assert(v.getSize() == 5);
    assert(v[2] == 'c');
    assert(v[4] == 'e');

    v.insert(5, 'q');
    assert(v.getSize() == 6);
    assert(v[0] == 'a');
    assert(v[5] == 'q');

    std::cout << "test_insert(): PASSED" << std::endl;
}

// Test removing elements at a specific index
void test_erase()
{
    Vector<float> v;
    v.push_back(1.1);
    std::cout << v << std::endl;
    v.push_back(2.2);
    std::cout << v << std::endl;
    v.push_back(3.3);
    std::cout << v << std::endl;
    v.push_back(4.4);
    std::cout << v << std::endl;
    v.pop_back();
    std::cout << v << std::endl;
    v.push_back(5.5);
    std::cout << v << std::endl;

    assert(v.getSize() == 4);
    std::cout << v[2] << std::endl;
    std::cout << v << std::endl;
    assert(v.At(2) == 3.3);

    // v.erase(0);
    // assert(v.getSize() == 2);
    // assert(v[0] == 2.2);
}

// Test accessing elements using the At() method
void test_accessing_elems()
{
    Vector<double> v;
    v.push_back(1.11);
    v.push_back(2.22);
    v.push_back(3.33);

    assert(v[1] == 2.22);
    assert(v.At(2) == 3.33);
    assert(v.Front() == 1.11);
    assert(v.Back() == 3.33);
    v[30] = 0.746;
    assert(v[30] == 0.746);
}

// Test clearing the vector
void test_clear()
{
    Vector<int> v;
    for (int i = 0; i < 200; ++i)
    {
        v.push_back(i);
    }

    v.clear();
    assert(v.empty() == true);
    assert(v.getSize() == 0);
    assert(v.getCapacity() > 0);
}

// Test exceptions
void test_exceptions()
{
    Vector<int> v;
    try
    {
        v.Front();
    }
    catch (const std::out_of_range &e)
    {
        assert(std::string(e.what()) == "Vector is empty!");
    }

    try
    {
        v.At(0);
    }
    catch (const std::out_of_range &e)
    {
        assert(std::string(e.what()) == "Index out of bounds!");
    }

    try
    {
        v.insert(1, 10);
    }
    catch (const std::out_of_range &e)
    {
        assert(std::string(e.what()) == "Index out of bounds!");
    }

    try
    {
        v.erase(0);
    }
    catch (const std::out_of_range &e)
    {
        assert(std::string(e.what()) == "Index out of bounds!");
    }

    try
    {
        v.pop_back();
    }
    catch (const std::out_of_range &e)
    {
        assert(std::string(e.what()) == "Pop back on empty vector!");
    }
}
