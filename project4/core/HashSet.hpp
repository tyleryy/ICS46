// HashSet.hpp
//
// ICS 46 Spring 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <memory>
#include <functional>
#include "Set.hpp"
#include <iostream>



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8, in which case
    // the new capacity should be determined by this formula:
    //
    //     capacity * 2 + 1
    //
    // In the case where the array is resized, this function runs in linear
    // time (with respect to the number of elements, assuming a good hash
    // function); otherwise, it runs in constant time (again, assuming a good
    // hash function).  The amortized running time is also constant.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns false.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:

    struct Node
    {
        ElementType value;
        std::shared_ptr<Node> next;
    };

    HashFunction hashFunction;
    int capacity;   
    std::shared_ptr<std::shared_ptr<Node>[]> hash_table; 
    std::shared_ptr<int[]> length_array;
    int element_count;

    // You'll no doubt want to add member variables and "helper" member
    // functions here.

    // void print_everything()
    // {
    //     std::cout << "------------------------" << std::endl;
    //     for(int i = 0; i < capacity; i++)
    //     {
    //         std::cout << "Index " << i << ": ";
    //         if(hash_table[i] != nullptr) 
    //         {
    //             std::shared_ptr<Node> curr = hash_table[i];
    //             while(curr != nullptr)
    //             {
    //                 std::cout << curr->value << ", "; 
    //                 curr = curr->next;
    //             }
    //         }
    //         else
    //         {
    //             std::cout << "nullptr";
    //         }
    //         std::cout << std::endl;
    //     }
    //     std::cout << "------------------------\n" << std::endl;

    //     std::cout << "------------------------" << std::endl;
    //     for(int i = 0; i < capacity; i++)
    //     {
    //         std::cout << "Index " << i << ": " << length_array[i] << std::endl;
    //     }
    //     std::cout << "------------------------\n" << std::endl;

    //     std::cout << "element_count: " << element_count << std::endl;
    //     std::cout << "capacity: " << capacity << std::endl;
    // }

    int get_hash_value(const ElementType& element) const;

    void reset_arrays(int cap);
    
    void copy_table(int cap, int old_cap, std::shared_ptr<std::shared_ptr<Node>[]> hash_t);
        
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}, capacity{DEFAULT_CAPACITY}, element_count{0}
{
    std::shared_ptr<int[]>dummy{new int[capacity]};
    length_array = dummy;
    hash_table = std::shared_ptr<std::shared_ptr<Node>[]>{new std::shared_ptr<Node>[capacity]};
    reset_arrays(capacity);
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}, capacity{s.capacity}, element_count{s.element_count}
{
    hashFunction = s.hashFunction;
    copy_table(s.capacity, s.capacity, s.hash_table);
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}, capacity{0}, hash_table{nullptr}, length_array{nullptr}, element_count{0}
{
    std::swap(hashFunction, s.hashFunction);
    std::swap(capacity, s.capacity);
    std::swap(hash_table, s.hash_table);
    std::swap(length_array, s.length_array);
    std::swap(element_count, s.element_count);


}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    if (this != &s)
    {
        copy_table(s.capacity,s.capacity,s.hash_table);
        capacity = s.capacity;
        element_count = s.element_count;

    }
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    std::swap(capacity, s.capacity);
    std::swap(hash_table, s.hash_table);
    std::swap(length_array, s.length_array);
    std::swap(element_count, s.element_count);

    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    int index = get_hash_value(element);
    bool found = false;

    
    std::shared_ptr<Node> curr_ll = hash_table[index];
    while(curr_ll != nullptr)
    {
        if(curr_ll->value == element)
        {
            found = true;
            break;
        }
        curr_ll = curr_ll->next;
    }
    if(not found)
    {
        std::shared_ptr<Node>next_node = hash_table[index];
        std::shared_ptr<Node> new_node{new Node{element, next_node} };
        hash_table[index] = new_node;
        length_array[index]++;
        element_count++;
    }
    
    if(element_count > capacity*.8)
    {
    
        int old_capacity = capacity;
        capacity = capacity*2 + 1;
        copy_table(capacity, old_capacity, hash_table);
    }

}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    if(element_count == 0)
    {
        return false;
    }
    int index = get_hash_value(element);
    
    std::shared_ptr<Node> curr_ll = hash_table[index];
    
    while(curr_ll != nullptr)
    {

        if(curr_ll->value == element)
        {
            return true;
        }
        curr_ll = curr_ll->next;
    }
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return element_count;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    if((index >= capacity) || index < 0)
        return 0;
    return length_array[index];
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    int ll_index = get_hash_value(element);
    if (contains(element))
    {
        if(ll_index == index)
        {
            return true;
        }
    }
    return false;
}

template <typename ElementType>
int HashSet<ElementType>::get_hash_value(const ElementType& element) const
{
    return hashFunction(element)%capacity;
}

template <typename ElementType>
void HashSet<ElementType>::reset_arrays(int cap)
{
    for(int i = 0; i < cap; i++)
    {
        hash_table[i] = nullptr;
        length_array[i] = 0;
    }
}

template <typename ElementType>
void HashSet<ElementType>::copy_table(int cap, int old_cap, std::shared_ptr<std::shared_ptr<Node>[]> hash_t)
{
    int index;
    std::shared_ptr<std::shared_ptr<Node>[]> copy_hash_table{new std::shared_ptr<Node>[capacity]};
    std::shared_ptr<int[]> copy_length_array{new int[cap]};
    length_array = copy_length_array;
    for(int i = 0; i < cap; i++)
    {
        copy_hash_table[i] = nullptr;
    }
    for(int i = 0; i < cap; i++)
    {
        length_array[i] = 0;
    }
    for(int i = 0; i < old_cap; i++)
    {
        if(hash_t[i] != nullptr)
        {
            std::shared_ptr<Node> curr_node = hash_t[i];
            while(curr_node != nullptr)
            {
                index = get_hash_value(curr_node->value);
                if(length_array[index] == 0)
                {
                    std::shared_ptr<Node> new_node = std::shared_ptr<Node>{new Node{curr_node->value, nullptr}};
                    copy_hash_table[index] = new_node;
                    //std::cout << new_node->value << std::endl;
                    curr_node = curr_node->next;
                }
                else
                {
                    std::shared_ptr<Node> next_node = copy_hash_table[index];
                    std::shared_ptr<Node> new_node = std::shared_ptr<Node>{new Node{curr_node->value, next_node}};
                    copy_hash_table[index] = new_node;
                    curr_node = curr_node->next;
                }
                length_array[index]++;
            }
        }
    }
    hash_table = copy_hash_table;
}






#endif

