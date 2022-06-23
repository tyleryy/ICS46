// SkipListSet.hpp
//
// ICS 46 Spring 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// A SkipListSet is an implementation of a Set that is a skip list, implemented
// as we discussed in lecture.  A skip list is a sequence of levels
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the keys and their
// values.  Instead, you'll need to implement your own dynamically-allocated
// nodes, with pointers connecting them.  You can, however, use other parts of
// the C++ Standard Library -- including <random>, notably.
//
// Each node should contain only two pointers: one to the node that follows it
// on the same level and another to the equivalent node on the level below it.
// Additional pointers use more memory but don't enable any techniques not
// enabled by the other two.
//
// A couple of utilities are included here: SkipListKind and SkipListKey.
// You can feel free to use these as-is and probably will not need to
// modify them, though you can make changes to them, if you'd like.

#ifndef SKIPLISTSET_HPP
#define SKIPLISTSET_HPP

#include <memory>
#include <optional>
#include <random>
#include "Set.hpp"
#include <iostream>




// SkipListKind indicates a kind of key: a normal one, the special key
// -INF, or the special key +INF.  It's necessary for us to implement
// the notion of -INF and +INF separately, since we're building a class
// template and not all types of keys would have a reasonable notion of
// -INF and +INF.

enum class SkipListKind
{
    Normal,
    NegInf,
    PosInf
};




// A SkipListKey represents a single key in a skip list.  It is possible
// to compare these keys using < or == operators (which are overloaded here)
// and those comparisons respect the notion of whether each key is normal,
// -INF, or +INF.

template <typename ElementType>
class SkipListKey
{
public:
    static SkipListKey normal(const ElementType& element);
    static SkipListKey negInf();
    static SkipListKey posInf();

    bool operator==(const SkipListKey& other) const;
    bool operator==(const ElementType& other) const;

    bool operator<(const SkipListKey& other) const;
    bool operator<(const ElementType& other) const;

    //remove this
    // void print_key()
    // {
    //     std::cout << *element << std::endl;
    // }

private:
    SkipListKey(SkipListKind kind, std::optional<ElementType> element);

private:
    SkipListKind kind;
    std::optional<ElementType> element;

};


template <typename ElementType>
SkipListKey<ElementType> SkipListKey<ElementType>::normal(const ElementType& element)
{
    return SkipListKey{SkipListKind::Normal, std::make_optional(element)};
}


template <typename ElementType>
SkipListKey<ElementType> SkipListKey<ElementType>::negInf()
{
    return SkipListKey{SkipListKind::NegInf, std::nullopt};
}


template <typename ElementType>
SkipListKey<ElementType> SkipListKey<ElementType>::posInf()
{
    return SkipListKey{SkipListKind::PosInf, std::nullopt};
}


template <typename ElementType>
SkipListKey<ElementType>::SkipListKey(SkipListKind kind, std::optional<ElementType> element)
    : kind{kind}, element{element}
{
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator==(const SkipListKey& other) const
{
    return kind == other.kind
        && (kind != SkipListKind::Normal || *element == *other.element);
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator==(const ElementType& other) const
{
    return kind == SkipListKind::Normal && *element == other;
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator<(const SkipListKey& other) const
{
    switch (kind)
    {
    case SkipListKind::NegInf:
        return other.kind != SkipListKind::NegInf;

    case SkipListKind::PosInf:
        return false;

    default: // SkipListKind::Normal
        return other.kind == SkipListKind::PosInf
            || (other.kind == SkipListKind::Normal && *element < *other.element);
    }
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator<(const ElementType& other) const
{
    return kind == SkipListKind::NegInf
        || (kind == SkipListKind::Normal && *element < other);
}



// The SkipListLevelTester class represents the ability to decide whether
// a key placed on one level of the skip list should also occupy the next
// level.  This is the "coin flip," so to speak.  Note that this is an
// abstract base class with one implementation, RandomSkipListLevelTester,
// just below it.  RandomSkipListLevelTester is what it sounds like: It
// makes the decision at random (with a 50/50 chance of deciding whether
// a key should occupy the next level).  However, by setting things up
// this way, we have a way to control things more carefully in our
// testing (as you can, as well).
//
// DO NOT MAKE CHANGES TO THE SIGNATURES OF THE MEMBER FUNCTIONS OF
// THE "level tester" CLASSES.  You can add new member functions or even
// whole new level tester classes, but the ones declared below are part
// of how we test your implementation, so they need to maintain the
// parts of the interface that are declared already.

template <typename ElementType>
class SkipListLevelTester
{
public:
    virtual ~SkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) = 0;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() = 0;
};


template <typename ElementType>
class RandomSkipListLevelTester : public SkipListLevelTester<ElementType>
{
public:
    RandomSkipListLevelTester();

    bool shouldOccupyNextLevel(const ElementType& element) override;
    std::unique_ptr<SkipListLevelTester<ElementType>> clone() override;

private:
    std::default_random_engine engine;
    std::bernoulli_distribution distribution;
};


template <typename ElementType>
RandomSkipListLevelTester<ElementType>::RandomSkipListLevelTester()
    : engine{std::random_device{}()}, distribution{0.5}
{
}


template <typename ElementType>
bool RandomSkipListLevelTester<ElementType>::shouldOccupyNextLevel(const ElementType& element)
{
    return distribution(engine);
}


template <typename ElementType>
std::unique_ptr<SkipListLevelTester<ElementType>> RandomSkipListLevelTester<ElementType>::clone()
{
    return std::unique_ptr<SkipListLevelTester<ElementType>>{
        new RandomSkipListLevelTester<ElementType>};
}




template <typename ElementType>
class SkipListSet : public Set<ElementType>
{
public:
    // Initializes an SkipListSet to be empty, with or without a
    // "level tester" object that will decide, whenever a "coin flip"
    // is needed, whether a key should occupy the next level above.
    SkipListSet();
    explicit SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester);

    // Cleans up the SkipListSet so that it leaks no memory.
    ~SkipListSet() noexcept override;

    // Initializes a new SkipListSet to be a copy of an existing one.
    SkipListSet(const SkipListSet& s);

    // Initializes a new SkipListSet whose contents are moved from an
    // expiring one.
    SkipListSet(SkipListSet&& s) noexcept;

    // Assigns an existing SkipListSet into another.
    SkipListSet& operator=(const SkipListSet& s);

    // Assigns an expiring SkipListSet into another.
    SkipListSet& operator=(SkipListSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a SkipListSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function runs in an expected time
    // of O(log n) (i.e., over the long run, we expect the average to be
    // O(log n)) with very high probability.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in an expected time of O(log n)
    // (i.e., over the long run, we expect the average to be O(log n))
    // with very high probability.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // levelCount() returns the number of levels in the skip list.
    unsigned int levelCount() const noexcept;


    // elementsOnLevel() returns the number of elements that are stored
    // on the given level of the skip list.  Level 0 is the bottom level;
    // level 1 is the one above level 0; and so on.  If the given level
    // doesn't exist, this function returns 0.  (Note that the -INF
    // and +INF shouldn't be counted.)
    unsigned int elementsOnLevel(unsigned int level) const noexcept;


    // isElementOnLevel() returns true if the given element is on the
    // given level, false otherwise.  Level 0 is the bottom level; level 1
    // is the one above level 0; and so on.  If the given level doesn't
    // exist, this function returns false.
    bool isElementOnLevel(const ElementType& element, unsigned int level) const;


private:
    std::unique_ptr<SkipListLevelTester<ElementType>> levelTester;

    struct Node
    {
        SkipListKey<ElementType> key;
        std::shared_ptr<Node> next;
        std::shared_ptr<Node> below;
    };

    struct LinkedList
    {
        unsigned int length;
        std::shared_ptr<Node> front;
        std::shared_ptr<Node> tail;
    };

private:
    std::shared_ptr<std::shared_ptr<LinkedList>[]> front_array;
    int tot_levels; //size
    int capacity;

    // void print_everything()
    // {
        
    //     for(int i = 0; i < tot_levels; i++)
    //     {
    //         std::shared_ptr<Node> ptr = front_array[i]->front;
    //         std::cout << "level: " << i << "\n" << std::endl;
    //         while(ptr != nullptr)
    //         {
    //             if(ptr->key == SkipListKey<ElementType>::negInf())
    //             {
    //                 std::cout << "negInf" << std::endl;
    //             }
    //             else if(ptr->key == SkipListKey<ElementType>::posInf())
    //             {
    //                 std::cout << "posInf" << std::endl;
    //             }
    //             else{
    //                 ptr->key.print_key();
    //             }
    //             ptr = ptr->next;
            
    //         }
    //         //test if they are posInf and negInf 

    //         //std::cout << "front: " << std::endl;
    //         //front_array[i]->front->key.print_key();
    //         //std::cout << "tail: " << std::endl; 
    //         //front_array[i]->tail->key.print_key();
    //         std::cout << "\nlength: " << front_array[i]->length << std::endl;
    //         std::cout << "----------------------------\n";
    //     }
    //     std::cout << "capacity: " << capacity << std::endl;
    //     std::cout << "tot_levels: " << tot_levels << std::endl;
    //     std::cout << "----------------------------\n";

    // }

    void assign_below_ptr(std::shared_ptr<Node> current, int current_level);

    void insert(SkipListKey<ElementType> key, int current_level);

    void copy_front_array(std::shared_ptr<std::shared_ptr<LinkedList>[]> front);

    void insert_into_skiplist(std::shared_ptr<Node> current, const ElementType& element, int current_level);

//inline functions
//definitions are within class due to errors when definitions are written outside

    std::shared_ptr<LinkedList> init_ll(int level)
    {
        std::shared_ptr<LinkedList> ll{new LinkedList};
        ll->length = 0;
        ll->front = std::make_shared<Node>(Node{SkipListKey<ElementType>::negInf(), nullptr, nullptr});
        ll->front->next = std::make_shared<Node>(Node{SkipListKey<ElementType>::posInf(), nullptr, nullptr});
        ll->tail = ll->front->next;
        if(level > 0)
        {
            ll->front->below = front_array[level-1]->front;
            ll->tail->below = front_array[level-1]->tail;
        }
        return ll; 
    }

    std::shared_ptr<Node> find_insertion_point(std::shared_ptr<Node> front, SkipListKey<ElementType> key)
    {
        while(true)
        {
            if(not (front->next->key < key))
            {
                return front;
            }
            else
            {
                front = front->next;
            }
        }
        return front;
    }

    std::shared_ptr<Node> insert_between(std::shared_ptr<Node> reader, std::shared_ptr<Node>& marker, std::shared_ptr<Node>& below_ptr) 
    {                                                                                    // below_ptr points to new node for reference in upper levels
        std::shared_ptr<Node> end = marker->next;
        std::shared_ptr<Node> new_node{new Node{reader->key, end, below_ptr}};
        marker->next = new_node;
        below_ptr = new_node;
        return below_ptr;
    }

    // You'll no doubt want to add member variables and "helper" member
    // functions here
};





template <typename ElementType>
SkipListSet<ElementType>::SkipListSet()
    : SkipListSet{std::make_unique<RandomSkipListLevelTester<ElementType>>()} //cheange back to SkipListSet
{
    tot_levels = 0;
    capacity = 10; //initial capacity of array
    std::shared_ptr<std::shared_ptr<LinkedList>[]> copy_array{new std::shared_ptr<LinkedList>[capacity]};
    front_array = copy_array;
    //front_array[0] = init_ll(0);
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester)
    : levelTester{std::move(levelTester)}
{
    tot_levels = 0;
    capacity = 10; //initial capacity of array
    std::shared_ptr<std::shared_ptr<LinkedList>[]> copy_array{new std::shared_ptr<LinkedList>[capacity]};
    front_array = copy_array;
    //front_array[0] = init_ll(0);
}


template <typename ElementType>
SkipListSet<ElementType>::~SkipListSet() noexcept
{
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(const SkipListSet& s)
    : tot_levels{s.tot_levels}, capacity{s.capacity}
{
    copy_front_array(s.front_array);
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(SkipListSet&& s) noexcept
    : front_array{nullptr}, tot_levels{0}, capacity{0}
{
    std::swap(front_array, s.front_array);
    std::swap(tot_levels, s.tot_levels);
    std::swap(capacity, s.capacity);
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(const SkipListSet& s)
{
    if (this != &s)
    {
        tot_levels = s.tot_levels;
        capacity = s.capacity;
        copy_front_array(s.front_array);
    }
    return *this;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(SkipListSet&& s) noexcept
{
    std::swap(front_array, s.front_array);
    std::swap(tot_levels, s.tot_levels);
    std::swap(capacity, s.capacity);
    return *this;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isImplemented() const noexcept
{
    return true;
}



template <typename ElementType>
void SkipListSet<ElementType>::add(const ElementType& element)
{
    //std::cout << "skip list height: " << tot_levels << std::endl;
    if(tot_levels == 0)
    {
        tot_levels++;
        front_array[0] = init_ll(0);
    }
    std::shared_ptr<Node> curr_list_index = front_array[tot_levels-1]->front;
    int current_level = tot_levels - 1;
    while (true)
    {
        if (curr_list_index->key == element)
        {
            break; 
        } 
        else if(not (curr_list_index->next->key < element)) // check if next key is greater, then move down
        {
            if (current_level > 0)
            {
                curr_list_index = curr_list_index->below;
                current_level--;
            }
            else
            {  
                // std::cout << current_level << std::endl;
                insert_into_skiplist(curr_list_index, element, current_level);
 
                break;
            }
        }
        else
        {
            curr_list_index = curr_list_index->next;
        }
    }
}


template <typename ElementType>
bool SkipListSet<ElementType>::contains(const ElementType& element) const
{
    //std::cout << "\n";
    if(tot_levels == 0)
    {
        return false;
    }
    std::shared_ptr<Node> curr_list_index = front_array[tot_levels-1]->front;
    //std::cout << "total levels: " << tot_levels << std::endl;
    while (curr_list_index != nullptr)
    {
        //std::cout << "curr_key: ";
        //curr_list_index->key.print_key();
        //std::cout << "next_key: ";
        //curr_list_index->next->key.print_key();
        if (curr_list_index->key == SkipListKey<ElementType>::normal(element)) 
        {
            //std::cout << "option: "<< 1 << std::endl;
            return true;
        } 
        else if((not (curr_list_index->next->key < SkipListKey<ElementType>::normal(element))) && (not (curr_list_index->next->key == SkipListKey<ElementType>::normal(element))) ) // check if next key is greater, then move down
        {
            //std::cout << "option: "<< 2 << std::endl;
            curr_list_index = curr_list_index->below;
        }
        else
        {
            //std::cout << "option: "<< 3 << std::endl;
            curr_list_index = curr_list_index->next;
        }
    }
    return false;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::size() const noexcept
{
    if(tot_levels == 0)
        return 0;
    return front_array[0]->length;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::levelCount() const noexcept
{
    return tot_levels;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::elementsOnLevel(unsigned int level) const noexcept
{
    if(level >= tot_levels)
        return 0;
    return front_array[level]->length;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isElementOnLevel(const ElementType& element, unsigned int level) const
{
    if(tot_levels == 0)
    {
        return false;
    }
    std::shared_ptr<Node> curr_list_index = front_array[tot_levels-1]->front;
    int current_level = tot_levels-1;
    while (curr_list_index != nullptr)
    {
        if (curr_list_index->key == element)
        {
            if (current_level == level)
                return true;
            return false;
        } 
        else if(not (curr_list_index->next->key < element) && (not (curr_list_index->next->key == element) )) // check if next key is greater, then move down
        {
            current_level--;
            curr_list_index = curr_list_index->below;
        }
        else
        {
            curr_list_index = curr_list_index->next;
        }
    }
    return false;
}

//private functions

template <typename ElementType>
void SkipListSet<ElementType>::assign_below_ptr(std::shared_ptr<Node> current, int current_level)
{
    std::shared_ptr<Node> below_list_ptr = front_array[current_level-1]->front;
    while(not (below_list_ptr->key == current->key))
    {
        below_list_ptr = below_list_ptr->next;
    }
    current->below = below_list_ptr;
}

template <typename ElementType>
void SkipListSet<ElementType>::insert(SkipListKey<ElementType> key, int current_level)
{
    std::shared_ptr<LinkedList> curr_ll = front_array[current_level];
    curr_ll->length++;
    std::shared_ptr<Node> current = find_insertion_point(curr_ll->front, key);

    std::shared_ptr<Node> next_node = current->next;
    std::shared_ptr<Node> new_node{new Node{key, next_node, nullptr}};
    current->next = new_node;

    if(current_level > 0)
    {
        current = current->next;
        assign_below_ptr(current, current_level);
    }
}

template <typename ElementType>
void SkipListSet<ElementType>::copy_front_array(std::shared_ptr<std::shared_ptr<LinkedList>[]> front)
{
    if(tot_levels > 0)
    {
        std::shared_ptr<std::shared_ptr<LinkedList>[]> copy_array{new std::shared_ptr<LinkedList>[capacity]};
        for(int i = 0; i < tot_levels; i++)
        {
            copy_array[i] = std::shared_ptr<LinkedList>{new LinkedList{front[i]->length, front[i]->front, front[i]->tail}};
        }
        front_array = copy_array;
    }
}

template <typename ElementType>
void SkipListSet<ElementType>::insert_into_skiplist(std::shared_ptr<Node> current, const ElementType& element, int current_level)
{
    bool coin;
    int curr_level = current_level;
    SkipListKey<ElementType> insert_key = SkipListKey<ElementType>::normal(element);
    insert(insert_key, curr_level);
    while(true)
    {
        coin = levelTester->shouldOccupyNextLevel(element);
        //std::cout << "coin: " << coin << std::endl;
        // std::cout << "capacity: " << capacity << std::endl;
        if(coin)
        {
            curr_level++;
            // std::cout << "curr_level: " << curr_level << std::endl;
            // std::cout << "capacity: " << capacity << std::endl;
            if (curr_level < tot_levels)
            {
                insert(insert_key, curr_level);
            }
            else if(curr_level < capacity)
            {
                tot_levels++;
                //std::cout << "curr levels: "  << curr_level << std::endl;
                front_array[curr_level] = init_ll(curr_level);             
                insert(insert_key, curr_level);
            }
            else
            {
                capacity*=2;
                copy_front_array(front_array);
                // std::cout << "herre" << std::endl;
                // print_everything();
                tot_levels++;
                front_array[curr_level] = init_ll(curr_level);
                insert(insert_key, curr_level);
            }
        }
        else
        {
            break; 
        }
    }
}



#endif

