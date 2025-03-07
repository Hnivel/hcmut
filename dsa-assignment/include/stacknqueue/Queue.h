/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Queue.h
 * Author: LTSACH
 *
 * Created on 19 August 2020, 22:33
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "list/DLinkedList.h"
#include "stacknqueue/IDeck.h"

template <class T>
class Queue : public IDeck<T>
{
public:
    class Iterator; // forward declaration

protected:
    DLinkedList<T> list;
    void (*deleteUserData)(DLinkedList<T> *);
    bool (*itemEqual)(T &lhs, T &rhs);

public:
    // (Finished)
    Queue(void (*deleteUserData)(DLinkedList<T> *) = 0, bool (*itemEqual)(T &, T &) = 0)
    {
        this->itemEqual = itemEqual;
        this->deleteUserData = deleteUserData;
        this->list = DLinkedList<T>(deleteUserData, itemEqual);
    }
    // (Finished)
    void push(T item)
    {
        this->list.add(item);
    }
    // (Finished)
    T pop()
    {
        if (this->empty() == true)
        {
            throw Underflow("Queue");
        }
        T item = this->list.removeAt(0);
        return item;
    }
    // (Finished)
    T &peek()
    {
        if (this->empty() == true)
        {
            throw Underflow("Queue");
        }
        return this->list.get(0);
    }
    // (Finished)
    bool empty()
    {
        return this->list.empty();
    }
    // (Finished)
    int size()
    {
        return this->list.size();
    }
    // (Finished)
    void clear()
    {
        this->list.clear();
    }
    // (Finished)
    bool remove(T item)
    {
        return this->list.removeItem(item);
    }
    // (Finished)
    bool contains(T item)
    {
        return this->list.contains(item);
    }
    string toString(string (*item2str)(T &) = 0)
    {
        stringstream os;
        os << "FRONT-TO-REAR: " << list.toString(item2str);
        return os.str();
    }
    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }

    ///

    Iterator front()
    {
        return Iterator(this, true);
    }
    Iterator rear()
    {
        return Iterator(this, false);
    }

private:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////

public:
    // Iterator: BEGIN
    class Iterator
    {
    private:
        Queue<T> *queue;
        typename DLinkedList<T>::Iterator listIt;

    public:
        Iterator(Queue<T> *queue = 0, bool begin = true)
        {
            this->queue = queue;
            if (begin)
            {
                if (queue != 0)
                    this->listIt = queue->list.begin();
                else
                    this->listIt = 0;
            }
            else
            {
                if (queue != 0)
                    this->listIt = queue->list.end();
                else
                    this->listIt = 0;
            }
        }
        Iterator &operator=(const Iterator &iterator)
        {
            this->queue = iterator.queue;
            this->listIt = iterator.listIt;
            return *this;
        }

        T &operator*()
        {
            return *(this->listIt);
        }
        bool operator!=(const Iterator &iterator)
        {
            return this->listIt != iterator.listIt;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            listIt++;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
        void remove(void (*removeItem)(T) = 0)
        {
            listIt.remove(removeItem);
        }
    };
    // Iterator: END
};

#endif /* QUEUE_H */
