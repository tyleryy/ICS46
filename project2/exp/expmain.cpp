// expmain.cpp
//
// Do whatever you'd like here.  This is intended to allow you to experiment
// with the code in the "app" directory or with any additional libraries
// that are part of the project, outside of the context of the main
// application or Google Test.

#include <iostream>
#include "DoublyLinkedList.hpp"
#include <string>
#include <vector>
#include <algorithm>
template<typename T>
void print(T value) 
{
    std::cout << value << std::endl; 
}



int main()
{

    DoublyLinkedList first_list = DoublyLinkedList<int>();

    first_list.addToStart(4);
    first_list.addToEnd(1);
    first_list.addToStart(5);
    first_list.addToEnd(6);
    first_list.removeFromEnd();
    DoublyLinkedList copy = first_list;
    DoublyLinkedList second_list = DoublyLinkedList<int>();
    second_list = first_list;
    first_list.removeFromStart();
    first_list.removeFromEnd();
    first_list.removeFromEnd();
    first_list.addToEnd(1);
    first_list.addToEnd(3);
    first_list.addToStart(5);
    first_list.removeFromEnd();
    first_list.removeFromEnd();

    auto iterator = second_list.iterator();
    print("iterator: ");

    iterator.moveToPrevious();
    print(iterator.isPastStart());
    print("");

    iterator.moveToNext();
    print(iterator.value());
    iterator.moveToNext();
    iterator.insertBefore(10);
    print(iterator.value());
    iterator.moveToPrevious();
    print(iterator.value());
    iterator.moveToNext();
    iterator.moveToNext();
    print(iterator.value());
    iterator.moveToNext();
    print(iterator.isPastEnd());

    iterator.moveToPrevious();
    print(iterator.value());
    iterator.remove();
    iterator.moveToPrevious();
    iterator.moveToNext();
    print(iterator.isPastEnd());
    iterator.moveToPrevious();
    print(iterator.value());
    iterator.remove();
    iterator.moveToPrevious();
    print(iterator.value());
    iterator.moveToPrevious();
    print(iterator.value());
    iterator.remove();
    print("isPast start: ");
    print(iterator.isPastStart());
    print(iterator.value());
    iterator.moveToPrevious();

    print(iterator.isPastStart());

    print("");

    print("list 1: ");
    print("size: ");
    print(first_list.size());
    print("isEmpty: ");
    print(first_list.isEmpty());
    print("last: ");
    print(first_list.last());
    print("front: ");
    print(first_list.first()); 

    print("");
    print("list 2: ");
    print("size: ");
    print(second_list.size());
    print("isEmpty: ");
    print(second_list.isEmpty());
    print("last: ");
    print(second_list.last());
    print("front: ");
    print(second_list.first()); 
    



	return 0;
}


