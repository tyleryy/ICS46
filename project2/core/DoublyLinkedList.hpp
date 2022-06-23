// DoublyLinkedList.hpp
//
// ICS 46 Spring 2022
// Project #2: Time Waits for No One
//
// DoublyLinkedList<ValueType> is a class template that implements a
// doubly-linked list with head and tail pointers, including two kinds
// of iterators: One of them allows viewing and modifying the list's
// contents, while the other allows only viewing them.
//
// Your goal is to implement the entire public interface *exactly* as
// specified below.  Do not modify the signatures of any of the public
// member functions (including the public member functions of the various
// iterator classes) in any way.  We will be running extensive unit
// tests against your implementation, which will not compile unless
// things remain completely unchanged.  As we did in Project #0, we've
// provided you a basic set of unit tests that briefly demonstrate how
// each of the member functions is required to behave; you'll find
// those in the "gtest" directory.
//
// All of the public member functions listed with "noexcept" in their
// signature must be implemented in a way that they never throw exceptions.
// All of the others are expected to make the "strong" exception guarantee,
// which means two things: (1) no memory has leaked, and (2) the contents
// of the list/iterator will not have visibly changed in the event that
// an exception has been thrown.
//
// The entire C++ Standard Library is off-limits in your implementation
// of this class.  DO NOT submit a version of this file (or any file
// that it includes) that includes any C++ Standard Library headers.
// (This includes things like adding a print() member function that
// requires <iostream>.)
//
// As is good custom in class templates, keep the interface separate
// from the implementation.  This means the bodies of member functions
// should not be written in the class declaration, but should appear
// below it.  I've placed "dummy" implementations of every public
// member function, though, of course, most of them don't do the
// right thing; but they will save you some typing and demonstrate
// the structure of what you should be writing.

#ifndef DOUBLYLINKEDLIST_HPP
#define DOUBLYLINKEDLIST_HPP

#include "EmptyException.hpp"
#include "IteratorException.hpp"



template <typename ValueType>
class DoublyLinkedList
{
    // The forward declarations of these classes allows us to establish
    // that they exist, but delay displaying all of the details until
    // later in the file.
    //
    // (This is generally a good style, with the most important details
    // appearing earlier in the class declaration.  That's the same
    // reason why we're implementing the bodies of the member functions
    // outside of the class declaration.)
public:
    class Iterator;
    class ConstIterator;


private:
    struct Node;


public:
    // Initializes this list to be empty.
    DoublyLinkedList() noexcept;

    // Initializes this list as a copy of an existing one.
    DoublyLinkedList(const DoublyLinkedList& list);

    // Initializes this list from an expiring one.
    DoublyLinkedList(DoublyLinkedList&& list) noexcept;


    // Destroys the contents of this list.
    virtual ~DoublyLinkedList() noexcept;


    // Replaces the contents of this list with a copy of the contents
    // of an existing one.
    DoublyLinkedList& operator=(const DoublyLinkedList& list);

    // Replaces the contents of this list with the contents of an
    // expiring one.
    DoublyLinkedList& operator=(DoublyLinkedList&& list) noexcept;


    // addToStart() adds a value to the start of the list, meaning that
    // it will now be the first value, with all subsequent elements still
    // being in the list (after the new value) in the same order.
    void addToStart(const ValueType& value);

    // addToEnd() adds a value to the end of the list, meaning that
    // it will now be the last value, with all subsequent elements still
    // being in the list (before the new value) in the same order.
    void addToEnd(const ValueType& value);


    // removeFromStart() removes a value from the start of the list, meaning
    // that the list will now contain all of the values *in the same order*
    // that it did before, *except* that the first one will be gone.
    // In the event that the list is empty, an EmptyException will be thrown.
    void removeFromStart();

    // removeFromEnd() removes a value from the end of the list, meaning
    // that the list will now contain all of the values *in the same order*
    // that it did before, *except* that the last one will be gone.
    // In the event that the list is empty, an EmptyException will be thrown.
    void removeFromEnd();


    // first() returns the value at the start of the list.  In the event that
    // the list is empty, an EmptyException will be thrown.  There are two
    // variants of this member function: one for a const DoublyLinkedList and
    // another for a non-const one.
    const ValueType& first() const;
    ValueType& first();


    // last() returns the value at the end of the list.  In the event that
    // the list is empty, an EmptyException will be thrown.  There are two
    // variants of this member function: one for a const DoublyLinkedList and
    // another for a non-const one.
    const ValueType& last() const;
    ValueType& last();


    // isEmpty() returns true if the list has no values in it, false
    // otherwise.
    bool isEmpty() const noexcept;


    // size() returns the number of values in the list.
    unsigned int size() const noexcept;

    



    // There are two kinds of iterators supported: Iterators and
    // ConstIterators.  They have similar characteristics; they both
    // allow you to see what values are in the list and move back and
    // forth between them.  The difference is that ConstIterators allow
    // you to see the elements but not modify them, while Iterators also
    // support modification of the list (both by modifying the elements
    // directly, and also by inserting or removing values at arbitrary
    // locations).
    //
    // At any given time, an iterator refers to a value in the list.
    // There are also two additional places it can refer: "past start"
    // and "past end", which are the positions directly before the
    // first value and directly after the last one, respectively.
    // Except with respect to those boundaries, they can be moved
    // both forward and backward.
    //
    // Note, too, that the reason we have a ConstIterator class instead
    // of just saying "const Iterator" is because a "const Iterator"
    // is something different: It's an Iterator object that you can't
    // modify (i.e., you can't move it around).  What a ConstIterator
    // holds constant isn't the iterator; it's the list that's protected
    // by it.


    // iterator() creates a new Iterator over this list.  It will
    // initially be referring to the first value in the list, unless the
    // list is empty, in which case it will be considered both "past start"
    // and "past end".
    Iterator iterator();


    // constIterator() creates a new ConstIterator over this list.  It will
    // initially be referring to the first value in the list, unless the
    // list is empty, in which case it will be considered both "past start"
    // and "past end".
    ConstIterator constIterator() const;


public:
    // The IteratorBase class is the base class for our two kinds of
    // iterators.  Because there are so many similarities between them,
    // we write those similarities in a base class, then inherit from
    // that base class to specify only the differences.
    class IteratorBase
    {
    public:
        // Initializes a newly-constructed IteratorBase to operate on
        // the given list.  It will initially be referring to the first
        // value in the list, unless the list is empty, in which case
        // it will be considered to be both "past start" and "past end".
        IteratorBase(const DoublyLinkedList& list) noexcept;


        // moveToNext() moves this iterator forward to the next value in
        // the list.  If the iterator is refrering to the last value, it
        // moves to the "past end" position.  If it is already at the
        // "past end" position, an IteratorException will be thrown.
        void moveToNext();


        // moveToPrevious() moves this iterator backward to the previous
        // value in the list.  If the iterator is refrering to the first
        // value, it moves to the "past start" position.  If it is already
        // at the "past start" position, an IteratorException will be thrown.
        void moveToPrevious();


        // isPastStart() returns true if this iterator is in the "past
        // start" position, false otherwise.
        bool isPastStart() const noexcept;


        // isPastEnd() returns true if this iterator is in the "past end"
        // position, false otherwise.
        bool isPastEnd() const noexcept;

    
    protected:
        // You may want protected member variables and member functions,
        // which will be accessible to the derived classes.

        //void iter_empty(); //may remove
        Node* current;
        unsigned int index;
        unsigned int size; 
        DoublyLinkedList* dllist;
    };


    class ConstIterator : public IteratorBase
    {
    public:
        // Initializes a newly-constructed ConstIterator to operate on
        // the given list.  It will initially be referring to the first
        // value in the list, unless the list is empty, in which case
        // it will be considered to be both "past start" and "past end".
        ConstIterator(const DoublyLinkedList& list) noexcept;


        // value() returns the value that the iterator is currently
        // referring to.  If the iterator is in the "past start" or
        // "past end" positions, an IteratorException will be thrown.
        const ValueType& value() const;

    private:
        // You may want private member variables and member functions.

        
    };


    class Iterator : public IteratorBase
    {
    public:
        // Initializes a newly-constructed Iterator to operate on the
        // given list.  It will initially be referring to the first
        // value in the list, unless the list is empty, in which case
        // it will be considered to be both "past start" and "past end".
        Iterator(DoublyLinkedList& list) noexcept;


        // value() returns the value that the iterator is currently
        // referring to.  If the iterator is in the "past start" or
        // "past end" positions, an IteratorException will be thrown.
        ValueType& value() const;


        // insertBefore() inserts a new value into the list before
        // the one to which the iterator currently refers.  If the
        // iterator is in the "past start" position, an IteratorException
        // is thrown.
        void insertBefore(const ValueType& value);


        // insertAfter() inserts a new value into the list after
        // the one to which the iterator currently refers.  If the
        // iterator is in the "past end" position, an IteratorException
        // is thrown.
        void insertAfter(const ValueType& value);


        // remove() removes the value to which this iterator refers,
        // moving the iterator to refer to either the value after it
        // (if moveToNextAfterward is true) or before it (if
        // moveToNextAfterward is false).  If the iterator is in the
        // "past start" or "past end" position, an IteratorException
        // is thrown.
        void remove(bool moveToNextAfterward = true);

    private:
        // You may want private member variables and member functions.

    };


private:
    // A structure that contains the vital parts of a Node in a
    // doubly-linked list, the value and two pointers: one pointing
    // to the previous node (or nullptr if there isn't one) and
    // one pointing to the next node (or nullptr if there isn't
    // one).

    struct Node
    {
        ValueType value;
        Node* prev;
        Node* next;
    };

    void copy_list(Node*& head, Node*& tail, const Node*& reader);


    // You can feel free to add private member variables and member
    // functions here; there's a pretty good chance you'll need some.

    Node* head;
    Node* tail;
};

namespace{
    template<typename T>
    void swapper(T& a, T& b)
    {
        T temp = a;
        a = b;
        b = temp;
    }
}

template <typename ValueType>
void DoublyLinkedList<ValueType>::copy_list(Node*& head, Node*& tail, const Node*& reader)
{
    //create head
    Node* previous;
    head = new Node{reader->value, nullptr};
    previous = head;
    tail = head;
    reader = reader->next;
    while (reader != nullptr) //create tail
    {
        Node* new_node = new Node{reader->value, previous};
        tail->next = new_node;
        tail = tail->next;
        previous = new_node; 
        reader = reader->next;
    }
    tail->next = nullptr; 
}

template <typename ValueType>
DoublyLinkedList<ValueType>::DoublyLinkedList() noexcept
    : head{nullptr}, tail{nullptr}
{

}


template <typename ValueType>
DoublyLinkedList<ValueType>::DoublyLinkedList(const DoublyLinkedList& list)
{
    const Node* reader = list.head;
    if (reader != nullptr)
    {
        copy_list(head, tail, reader);
    }
    else
    {
        head = nullptr;
        tail = nullptr;
    }
}


template <typename ValueType>
DoublyLinkedList<ValueType>::DoublyLinkedList(DoublyLinkedList&& list) noexcept
    : head{nullptr}, tail{nullptr}
{
    swapper(head, list.head);
    swapper(tail, list.tail);
}


template <typename ValueType>
DoublyLinkedList<ValueType>::~DoublyLinkedList() noexcept
{
    Node* current;
    while (head != nullptr)
    {
        current = head;
        head = head->next;
        delete current;
    }
}

//fix this
template <typename ValueType>
DoublyLinkedList<ValueType>& DoublyLinkedList<ValueType>::operator=(const DoublyLinkedList& list)
{
    if (this != &list)
    {
        const Node* reader = list.head;
        if (reader != nullptr)
        {
            Node* head_ref = head;

            Node* copy_head;
            Node* copy_tail;

            copy_list(copy_head, copy_tail, reader);

        //encapsulate this
            Node* current;
            while(head_ref != nullptr)
            {
                current = head_ref;
                head_ref = head_ref->next;
                delete current;
            }

            head = copy_head;
            tail = copy_tail;
        }
        else
        {
            head = nullptr;
            tail = nullptr;
        }

    }
    return *this;
}


template <typename ValueType>
DoublyLinkedList<ValueType>& DoublyLinkedList<ValueType>::operator=(DoublyLinkedList&& list) noexcept
{
    swapper(head, list.head);
    swapper(tail, list.tail);
    return *this;
}



template <typename ValueType>
void DoublyLinkedList<ValueType>::addToStart(const ValueType& value)
{
    Node* new_node = new Node{value, nullptr, head};
    if (head != nullptr)
        head->prev = new_node;
    head = new_node;
    if (tail == nullptr)
        tail = head;
    
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::addToEnd(const ValueType& value)
{
    
    Node* new_node = new Node{value, tail, nullptr};
    if (tail != nullptr)
        tail->next = new_node;
    tail = new_node;
    if (head == nullptr)
        head = tail;
    
    
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::removeFromStart()
{
    if (head == nullptr)
        throw EmptyException();
    Node* start = head;
    head = head->next;
    if (head != nullptr)
        head->prev = nullptr;
    else
        tail = nullptr;
    delete start;
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::removeFromEnd()
{
    if (tail == nullptr)
        throw EmptyException();
    Node* end = tail;
    tail = tail->prev;
    if (tail != nullptr)
        tail->next = nullptr;
    else
        head = nullptr;
    delete end;
}


template <typename ValueType>
const ValueType& DoublyLinkedList<ValueType>::first() const
{
    // Note that this is an awful thing I'm doing here, but I needed
    // something that would make this code compile.  You're definitely
    // going to want to replace this with something else before you
    // ever call this member function and expect it to work.  This
    // version *will* leak memory and *will* return an undefined value.
    if (head == nullptr)
        throw EmptyException();
    
    return head->value;
}


template <typename ValueType>
ValueType& DoublyLinkedList<ValueType>::first()
{
    // Note that this is an awful thing I'm doing here, but I needed
    // something that would make this code compile.  You're definitely
    // going to want to replace this with something else before you
    // ever call this member function and expect it to work.  This
    // version *will* leak memory and *will* return an undefined value.
    if (head == nullptr)
        throw EmptyException();

    return head->value;
}


template <typename ValueType>
const ValueType& DoublyLinkedList<ValueType>::last() const
{
    // Note that this is an awful thing I'm doing here, but I needed
    // something that would make this code compile.  You're definitely
    // going to want to replace this with something else before you
    // ever call this member function and expect it to work.  This
    // version *will* leak memory and *will* return an undefined value.
    if (tail == nullptr)
        throw EmptyException();

    return tail->value;
}


template <typename ValueType>
ValueType& DoublyLinkedList<ValueType>::last()
{
    // Note that this is an awful thing I'm doing here, but I needed
    // something that would make this code compile.  You're definitely
    // going to want to replace this with something else before you
    // ever call this member function and expect it to work.  This
    // version *will* leak memory and *will* return an undefined value.
 
    if (tail == nullptr)
        throw EmptyException();

    return tail->value;
}


template <typename ValueType>
unsigned int DoublyLinkedList<ValueType>::size() const noexcept
{
    unsigned int counter = 0;
    Node* current = head;
    while (current != nullptr){
        counter++;
        current = current->next;
    }
    return counter;
}


template <typename ValueType>
bool DoublyLinkedList<ValueType>::isEmpty() const noexcept
{ 
    if (head == nullptr)
        return true;
    return false;
}


template <typename ValueType>
typename DoublyLinkedList<ValueType>::Iterator DoublyLinkedList<ValueType>::iterator()
{
    return Iterator{*this};
}


template <typename ValueType>
typename DoublyLinkedList<ValueType>::ConstIterator DoublyLinkedList<ValueType>::constIterator() const
{
    return ConstIterator{*this};
}


template <typename ValueType>
DoublyLinkedList<ValueType>::IteratorBase::IteratorBase(const DoublyLinkedList& list) noexcept
{
    current = list.head;
    if (current != nullptr)
        index = 0;
    else
        index = -1;
    size = list.size();
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::IteratorBase::moveToNext()
{
    if ((index == size) || (size == 0))
        throw IteratorException();
    
    if (index < size-1 && index >= 0)
        current = current->next;
    index++;
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::IteratorBase::moveToPrevious()
{
    if ((index == -1) || (size == 0))
        throw IteratorException();
    
    
    if (index > 0 && index < size)
        current = current->prev;
    index--;
}


template <typename ValueType>
bool DoublyLinkedList<ValueType>::IteratorBase::isPastStart() const noexcept
{
    if ((index == -1) || (size == 0))
        return true;
    return false;
}


template <typename ValueType>
bool DoublyLinkedList<ValueType>::IteratorBase::isPastEnd() const noexcept
{
    if ((index == size) || (size == 0))
        return true;
    return false;
}


template <typename ValueType>
DoublyLinkedList<ValueType>::ConstIterator::ConstIterator(const DoublyLinkedList& list) noexcept
    : IteratorBase(list)
{
}


template <typename ValueType>
const ValueType& DoublyLinkedList<ValueType>::ConstIterator::value() const
{
    // Note that this is an awful thing I'm doing here, but I needed
    // something that would make this code compile.  You're definitely
    // going to want to replace this with something else before you
    // ever call this member function and expect it to work.  This
    // version *will* leak memory and *will* return an undefined value.

    if ((IteratorBase::index == IteratorBase::size) || (IteratorBase::size == 0) || (IteratorBase::index == -1))
        throw IteratorException();
    return IteratorBase::current->value;

}


template <typename ValueType>
DoublyLinkedList<ValueType>::Iterator::Iterator(DoublyLinkedList& list) noexcept
    : IteratorBase(list)
{
    IteratorBase::dllist = &list;
}


template <typename ValueType>
ValueType& DoublyLinkedList<ValueType>::Iterator::value() const
{
    // Note that this is an awful thing I'm doing here, but I needed
    // something that would make this code compile.  You're definitely
    // going to want to replace this with something else before you
    // ever call this member function and expect it to work.  This
    // version *will* leak memory and *will* return an undefined value.

    if ((IteratorBase::index == IteratorBase::size) || (IteratorBase::size == 0) || (IteratorBase::index == -1))
        throw IteratorException();
    return IteratorBase::current->value;
}

template <typename ValueType>
void DoublyLinkedList<ValueType>::Iterator::insertBefore(const ValueType& value)
{

    if ((IteratorBase::index == -1) || (IteratorBase::size == 0))
        throw IteratorException();
    Node* new_node;
    if (IteratorBase::index == IteratorBase::size)
    {
        new_node = new Node{value, IteratorBase::current, nullptr};
        IteratorBase::current->next = new_node;
        IteratorBase::dllist->tail = new_node;
        IteratorBase::current = new_node;
    }
    else if (IteratorBase::current == IteratorBase::dllist->head)
    {
        new_node = new Node{value, nullptr, IteratorBase::current};
        IteratorBase::dllist->head = new_node;
        IteratorBase::current->prev = new_node;
    }
    else{
        Node* prev_node = IteratorBase::current->prev;
        new_node = new Node{value, prev_node, IteratorBase::current}; 
        prev_node->next = new_node;
        IteratorBase::current->prev = new_node;
    }
    

    IteratorBase::index++;
    IteratorBase::size++;

    


}


template <typename ValueType>
void DoublyLinkedList<ValueType>::Iterator::insertAfter(const ValueType& value)
{
    if ((IteratorBase::index == IteratorBase::size) || (IteratorBase::size == 0))
        throw IteratorException(); 

    Node* new_node;
    if (IteratorBase::index == -1)
    {
        new_node = new Node{value, nullptr, IteratorBase::current};
        IteratorBase::current->prev = new_node;
        IteratorBase::dllist->head = new_node;
        IteratorBase::current = new_node;
    }
    else if (IteratorBase::current == IteratorBase::dllist->tail)
    {
        new_node = new Node{value, IteratorBase::current, nullptr};
        IteratorBase::dllist->tail = new_node;
        IteratorBase::current->next = new_node;
    }
    else
    {
        Node* next_node = IteratorBase::current->next; 
        new_node = new Node{value, IteratorBase::current, next_node}; 
        next_node->prev = new_node; 
        IteratorBase::current->next = new_node;
    }
    IteratorBase::size++;
}


//manage when list is emptied and only end nodes remain
template <typename ValueType>
void DoublyLinkedList<ValueType>::Iterator::remove(bool moveToNextAfterward)
{
    if ((IteratorBase::index == -1) || (IteratorBase::size == 0) || (IteratorBase::index == IteratorBase::size))
        throw IteratorException();

    Node* temp = IteratorBase::current;
    if (IteratorBase::size > 1)
    {
        if (IteratorBase::current == IteratorBase::dllist->head)
        {
            if (not moveToNextAfterward)
                IteratorBase::index--;
            IteratorBase::current = Iterator::current->next;
            IteratorBase::dllist->head = IteratorBase::current;
            IteratorBase::current->prev = nullptr;
        }
        else if (IteratorBase::current == IteratorBase::dllist->tail)
        {
            if (not moveToNextAfterward)
                IteratorBase::index--;
            IteratorBase::current = Iterator::current->prev;
            IteratorBase::dllist->tail = IteratorBase::current;
            IteratorBase::current->next = nullptr;
        }
        else{
            Node* prev_node = IteratorBase::current->prev;
            Node* next_node = IteratorBase::current->next;

            if (not moveToNextAfterward)
            {
                IteratorBase::index--;
                IteratorBase::current = prev_node;
            }
            else
                IteratorBase::current = next_node;
            prev_node->next = next_node;
            next_node->prev = prev_node;
        }
    }
    else
    {
        IteratorBase::current = nullptr;
        IteratorBase::dllist->head = nullptr;
        IteratorBase::dllist->tail = nullptr;
        IteratorBase::index--;
    }
    IteratorBase::size--;

    delete temp;
}



#endif

