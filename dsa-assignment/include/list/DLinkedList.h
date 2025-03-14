/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END
    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
    class BWDIterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        BWDIterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->tail->prev; // Start from the last element
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->head; // Set to head (before first element)
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        BWDIterator &operator=(const BWDIterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }

        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->next; // MUST use next for backward iterator
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*() { return pNode->data; }

        bool operator!=(const BWDIterator &iterator)
        {
            return pNode != iterator.pNode;
        }

        // Prefix ++ overload (move to the previous node)
        BWDIterator &operator++()
        {
            pNode = pNode->prev;
            return *this;
        }

        // Postfix ++ overload
        BWDIterator operator--(int)
        {
            BWDIterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
};
//////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////
// Constructor (Done) //
template <class T>
DLinkedList<T>::DLinkedList(void (*deleteUserData)(DLinkedList<T> *), bool (*itemEqual)(T &, T &))
{
    this->head = new Node();
    this->tail = new Node();
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;
    this->count = 0;
    this->head->next = this->tail;
    this->tail->prev = this->head;
}
// Copy Constructor (Done) //
template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;
    //
    this->copyFrom(list);
}
// Operator = (Done) //
template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    if (this != &list)
    {
        this->removeInternalData();
        this->copyFrom(list);
    }
    return *this;
}
// Destructor (Done) //
template <class T>
DLinkedList<T>::~DLinkedList()
{
    this->removeInternalData();
    if (this->head != nullptr)
    {
        delete this->head;
    }
    if (this->tail != nullptr)
    {
        delete this->tail;
    }
}
// Add the element at the end of the list (Done) //
template <class T>
void DLinkedList<T>::add(T e)
{
    Node *temporary = new Node(e, nullptr, nullptr);
    this->tail->prev->next = temporary;
    temporary->prev = this->tail->prev;
    temporary->next = this->tail;
    this->tail->prev = temporary;
    this->count++;
}
// Add the element at position index in the list (Done) //
template <class T>
void DLinkedList<T>::add(int index, T e)
{
    if (index < 0 || index > this->count)
    {
        throw std::out_of_range("Index is out of range!");
    }
    Node *temporary = new Node(e, nullptr, nullptr);
    Node *previous = this->getPreviousNodeOf(index);
    temporary->next = previous->next;
    temporary->prev = previous;
    previous->next->prev = temporary;
    previous->next = temporary;
    this->count++;
}
// Return the previous element of the element at position index (Done) //
template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
{
    /**
     * Returns the node preceding the specified index in the doubly linked list.
     * If the index is in the first half of the list, it traverses from the head; otherwise, it traverses from the tail.
     * Efficiently navigates to the node by choosing the shorter path based on the index's position.
     */
    if (index < 0 || index > this->count)
    {
        throw std::out_of_range("Index is out of range!");
    }
    if (index < this->count / 2)
    {
        Node *current = this->head;
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }
        return current;
    }
    else
    {
        Node *current = this->tail->prev;
        for (int i = this->count; i > index; i--)
        {
            current = current->prev;
        }
        return current;
    }
}
// Remove the element at position index in the list //
template <class T>
T DLinkedList<T>::removeAt(int index)
{
    if (index < 0 || index >= this->count)
    {
        throw std::out_of_range("Index is out of range!");
    }
    Node *previous = getPreviousNodeOf(index);
    Node *remove_node = previous->next;
    T item = remove_node->data;
    previous->next = remove_node->next;
    remove_node->next->prev = previous;
    delete remove_node;
    this->count--;
    return item;
}
// Check if the list is empty (Done) //
template <class T>
bool DLinkedList<T>::empty()
{
    return this->count == 0;
}
// Return the number of elements currently in the list (Done) //
template <class T>
int DLinkedList<T>::size()
{
    return this->count;
}
// Clear the list (Done) //
template <class T>
void DLinkedList<T>::clear()
{
    this->removeInternalData();
}
// Returns a reference to the element at position index (Done) //
template <class T>
T &DLinkedList<T>::get(int index)
{
    if (index < 0 || index >= this->count)
    {
        throw std::out_of_range("Index is out of range!");
    }
    Node *previous = this->getPreviousNodeOf(index);
    return previous->next->data;
}
// Returns the index of the first element with a value equal to item (Done) //
template <class T>
int DLinkedList<T>::indexOf(T item)
{
    int index = 0;
    for (DLinkedList<T>::Iterator it = this->begin(); it != this->end(); it++)
    {
        if (this->equals(*it, item, this->itemEqual))
        {
            return index;
        }
        index++;
    }
    return -1;
}
// Remove the first element with a value equal to item in the list (Done) //
template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    int index = this->indexOf(item);
    if (index != -1)
    {
        T remove_item = this->removeAt(index);
        if (removeItemData != 0)
        {
            removeItemData(remove_item);
        }
        return true;
    }
    return false;
}
// Check if the list contains item (Done)
template <class T>
bool DLinkedList<T>::contains(T item)
{
    return this->indexOf(item) != -1;
}
// Returns a string representation of the elements in the list (Done) //
template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the list into a string representation, where each element is formatted using a user-provided function.
     * If no custom function is provided, it directly uses the element's default string representation.
     * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
     * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
     */
    stringstream result;
    result << "[";
    Node *current = this->head->next;
    while (current != this->tail)
    {
        if (item2str != nullptr)
        {
            result << item2str(current->data);
        }
        else
        {
            result << current->data;
        }
        if (current->next != this->tail)
        {
            result << ", ";
        }
        current = current->next;
    }
    result << "]";
    return result.str();
}
// Copy (Done) //
template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{
    /**
     * Copies the contents of another doubly linked list into this list.
     * Initializes the current list to an empty state and then duplicates all data and pointers from the source list.
     * Iterates through the source list and adds each element, preserving the order of the nodes.
     */
    this->clear();
    Node *current = list.head->next;
    while (current != list.tail)
    {
        this->add(current->data);
        current = current->next;
    }
    this->itemEqual = list.itemEqual;
    this->deleteUserData = list.deleteUserData;
}
// Remove Internal Data (Done) //
template <class T>
void DLinkedList<T>::removeInternalData()
{
    if (this->head->next != this->tail && this->deleteUserData != nullptr)
    {
        this->deleteUserData(this);
    }
    Node *current = this->head->next;
    while (current != this->tail)
    {
        Node *next = current->next;
        delete current;
        current = next;
    }
    this->head->next = this->tail;
    this->tail->prev = this->head;
    this->count = 0;
}

#endif /* DLINKEDLIST_H */
