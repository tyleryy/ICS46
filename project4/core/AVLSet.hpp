// AVLSet.hpp
//
// ICS 46 Spring 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <memory>
#include <functional>
#include "Set.hpp"
#include <iostream>



template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    ~AVLSet() noexcept override;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const noexcept;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    
    struct Node
    {
        ElementType value;
        Node* left = nullptr;
        Node* right = nullptr;
        int height;
    };

    int tree_height;
    int count;
    bool balanced;

    Node* root; 


    void print_everything(Node* base)
    {
        if (base != nullptr)
        {
            std::cout << "height: " << base->height << " value: " << base->value << std::endl;
            print_everything(base->right);
            print_everything(base->left);
        }
        else
        {
            std::cout << "nullptr" << std::endl;
        }
    }


    bool isLeaf(Node* node);

    void clear_all(Node* node);

    void m_preorder(Node* base, VisitFunction visit) const;

    void m_postorder(Node* base, VisitFunction visit) const;

    void m_inorder(Node* base, VisitFunction visit) const;

    bool search(const ElementType& element, Node* base) const;

    int add_node(const ElementType& element, Node*& base);

    int get_balanced_factor(Node* base);

    int get_height(Node* base);

    int reset_heights(Node* base);

//inline functions
//gives error when I try to write definition outside of class
//  -says Node is not recognized

    Node* LLrotation(Node* base)
    {
        Node* r = base;
        Node* lr = r->left;
        r->left = lr->right;
        lr->right = r;
        return lr;
    }

    Node* RRrotation(Node* base)
    {
        Node* r = base;
        Node* rr = r->right;
        r->right = rr->left;
        rr->left = r;
        return rr; 
    }

    Node* RLrotation(Node* base)
    {
        Node* r = base;
        Node* rr = r->right;
        Node* rrl = r->right->left;
        r->right = rrl->left;
        rr->left = rrl->right;
        rrl->left = r;
        rrl->right = rr;
        return rrl; 
    }

    Node* LRrotation(Node* base)
    {
        Node* r = base;
        Node* rl = r->left;
        Node* rlr = r->left->right;
        r->left = rlr->right;
        rl->right = rlr->left;
        rlr->right = r;
        rlr->left = rl;
        return rlr; 
    }

    Node* balanced_add(const ElementType& element, Node*& base)
    {
        if(base == nullptr)
        {
            base = new Node{element, nullptr, nullptr, 1};
            return base;
        }
        else if(element > base->value) //right
        {
            base->right = balanced_add(element, base->right);
        }
        else //left
        {
            base->left = balanced_add(element, base->left);
        }
        base->height = get_height(base);

        if((get_balanced_factor(base) == 2) && (get_balanced_factor(base->left)==1))
        {
            base = LLrotation(base);
            base->height = reset_heights(base);
        }
        else if((get_balanced_factor(base) == -2) && (get_balanced_factor(base->right) == -1))
        {
            base = RRrotation(base);
            base->height = reset_heights(base);
        }
        else if((get_balanced_factor(base) == -2) && (get_balanced_factor(base->right) == 1))
        {
            base = RLrotation(base);
            base->height = reset_heights(base);
        }
        else if((get_balanced_factor(base)) == 2 && (get_balanced_factor(base->left) == -1))
        {
            base = LRrotation(base);
            base->height = reset_heights(base);
        }        

        return base;
    }

    Node* copy_tree(Node*& writer, Node* reader)
    {
        if(reader != nullptr)
        {
            Node* new_node = new Node{reader->value, nullptr, nullptr, reader->height};
            writer = new_node;
            writer->left = copy_tree(writer->left, reader->left);
            writer->right = copy_tree(writer->right, reader->right);
        }
        else
        {
            writer = nullptr;
        }
        return writer;
    }

};

//tree code

template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
    : tree_height{-1}, count{0}, balanced{shouldBalance}, root{nullptr}
{
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    clear_all(root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
    : tree_height{s.tree_height}, count{s.count}, balanced{s.balanced}
{
    Node* copy_root = nullptr;
    copy_root = copy_tree(copy_root, s.root); 
    root = copy_root;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
    : tree_height{0}, count{0}, balanced{0}, root{nullptr}
{
    std::swap(tree_height, s.tree_height);
    std::swap(count, s.count);
    std::swap(root, s.root);
    std::swap(balanced, s.balanced);
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if(this == &s)
    {
        tree_height = s.tree_height;
        count = s.count;
        Node* copy_root = nullptr;
        copy_root = copy_tree(copy_root, s.root); 
        clear_all(root);
        root = copy_root;
        balanced = s.balanced;
    }
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    std::swap(tree_height, s.tree_height);
    std::swap(count, s.count);
    std::swap(root, s.root);
    std::swap(balanced, s.balanced);
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    if(balanced)
    {
        root = balanced_add(element, root);
        tree_height = root->height - 1;
    }
    else
    {
        int new_height = add_node(element, root);
        if(root->height < new_height)
        {
            root->height = new_height;
        }
        tree_height = root->height;
    }
    count++;
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    return search(element, root);
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return count;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const noexcept
{
    return tree_height;
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    m_preorder(root, visit);
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    m_inorder(root, visit);
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    m_postorder(root, visit);
}


//private functions

template <typename ElementType>
bool AVLSet<ElementType>::isLeaf(Node* node)
{
    if((node->left == nullptr) && (node->right == nullptr))
        return true;
    return false; 
}

template <typename ElementType>
void AVLSet<ElementType>::clear_all(Node* node)
{
    if(node != nullptr)
    {
        clear_all(node->left);
        clear_all(node->right);
        delete node;
    }
}

template <typename ElementType>
void AVLSet<ElementType>::m_preorder(Node* base, VisitFunction visit) const
{
    if(base != nullptr)
    {
        visit(base->value);
        m_preorder(base->left, visit);
        m_preorder(base->right, visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::m_postorder(Node* base, VisitFunction visit) const
{
    if(base != nullptr)
    {
        m_postorder(base->left, visit);
        m_postorder(base->right, visit);
        visit(base->value);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::m_inorder(Node* base, VisitFunction visit) const
{
    if(base != nullptr)
    {
        m_inorder(base->left, visit);
        visit(base->value);
        m_inorder(base->right, visit);
    }
}

template <typename ElementType>
bool AVLSet<ElementType>::search(const ElementType& element, Node* base) const
{
    if(base == nullptr)
        return false;
    else if(base->value == element)
        return true;
    else if(element > base->value)
        return search(element, base->right);
    else
        return search(element, base->left);
}

template <typename ElementType>
int AVLSet<ElementType>::add_node(const ElementType& element, Node*& base)
{
    int new_height;
    if(base == nullptr)
    {
        base = new Node{element, nullptr, nullptr, 0};
        return 0;
    }
    else if(element > base->value) //right
    {
        new_height = 1 + add_node(element, base->right);
        if(new_height > base->height)
            base->height = new_height;
        return new_height;
    }
    else if(element < base->value) //left
    {
        new_height = 1 + add_node(element, base->left);
        if(new_height > base->height)
            base->height = new_height;
        return new_height;
    }
    return 0;
}

template <typename ElementType>
int AVLSet<ElementType>::get_balanced_factor(Node* base)
{
    if(base->left && base->right)
    {
        return base->left->height - base->right->height;
    }
    else if(base->left && base->right == nullptr)
    {
        return base->left->height ; 
    }
    else if(base->left== nullptr && base->right)
    {
        return -base->right->height;
    }
    return 0;
}

template <typename ElementType>
int AVLSet<ElementType>::get_height(Node* base)
{
    if((base->left != nullptr) && (base->right != nullptr))
    {
        if (base->left->height < base->right->height)
            return base->right->height + 1;
        else 
            return  base->left->height + 1;
    }
    else if((base->left != nullptr) && (base->right == nullptr))
    {
        return base->left->height + 1;
    }
    else if((base->left == nullptr) && (base->right != nullptr))
    {
        return base->right->height + 1;
    }
    return 0;
}

template <typename ElementType>
int AVLSet<ElementType>::reset_heights(Node* base)
{
    if(base == nullptr)
        return 0;
    int new_height = std::max(1 + reset_heights(base->left), 1 + reset_heights(base->right));
    base->height = new_height;
    return new_height;
}


#endif

