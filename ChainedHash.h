//
// Created by Gharam on 6/10/2020.
//

#ifndef WET2_CHAINEDHASH_H
#define WET2_CHAINEDHASH_H

#include "BidirectionalNode.h"
#define INITIAL_SIZE 10
#define EXPAND_PARAM 2
#define SHRINK_PARAM 0.25

template<class T>
class hashTable {
    int tableSize;
    int numOfElem;
    BidirectionalNode<T> **arr;
public:
    hashTable<T>() : tableSize(INITIAL_SIZE), numOfElem(0), arr(new BidirectionalNode<T> *[INITIAL_SIZE]) {
        for (int i = 0; i < INITIAL_SIZE; i++) {
            arr[i] = new BidirectionalNode<T>();
        }
    }

    ~hashTable() {
        for (int i = 0; i < tableSize; i++) {
            delete arr[i];
            arr[i] = nullptr;
        }
        delete[] arr;
        arr = nullptr;
    }

    bool exists(int i) {
        int in_table = i % tableSize;
        Node<T> *node = arr[in_table]->getFirst();
        while (node != nullptr) {
            if (node->i == i) {
                return true;
            }
            node = node->getNext();
        }
        return false;
    }

    Node<T> *Member(int i) {
        int in_table = i % tableSize;
        Node<T> *node = arr[in_table]->getFirst();
        while (node != nullptr) {
            if (node->i == i) {
                return node;
            }
            node = node->getNext();
        }
        return nullptr;
    }

    bool insert(int i) {
        if (exists(i)) {
            return 0;
        }
        int in_table = i % tableSize;
        Artist* to_add = new Artist(i);
        arr[in_table]->insert(arr[in_table]->getTail(), to_add, i);
        numOfElem++;
        reSize();
        return 1;
    }

    void Delete(int i, Node<T>* to_remove) {
        if (!exists(i)) {
            return;
        }
        int index = i % tableSize;
        arr[index]->remove(to_remove);
        numOfElem--;
        reSize();
    }

private:
    void reSize() {

        if (numOfElem > 0.5 * tableSize) {

            int newTableSize = EXPAND_PARAM * tableSize;

            BidirectionalNode<T> **new_arr = new BidirectionalNode<T> *[newTableSize];

            for (int i = 0; i < newTableSize; i++) {
                new_arr[i] = new BidirectionalNode<T>();
        }
            for (int i = 0; i < tableSize; i++) {
                Node<T> *current = arr[i]->getFirst();
                while (current != nullptr) {
                    Node<T> *next = current->next;
                    int in_table = (current->i) % newTableSize;
                    new_arr[in_table]->insert(new_arr[in_table]->getTail(), current->getData(), current->i);
                    current->setData(nullptr);
                    current = next;
                }
                //arr[i]->setTail(arr[i]->getHead());
                //arr[i]->getHead()->setNext(nullptr);
                delete arr[i];
            }
            tableSize = newTableSize;
            delete[] arr; //TODO
            arr = new_arr;
            return;
        }

        if ((numOfElem < 0.25 * tableSize) && (tableSize != INITIAL_SIZE)) {
            int newTableSize = (int) tableSize * SHRINK_PARAM;
            BidirectionalNode<T> **newArr = new BidirectionalNode<T> *[newTableSize];
            for (int i = 0; i < newTableSize; i++) {
                newArr[i] = new BidirectionalNode<T>();
            }
            for (int i = 0; i < tableSize; i++) {
                Node<T> *current = arr[i]->getFirst();
                while (current != nullptr) {
                    int in_table = (current->i) % newTableSize;
                    newArr[in_table]->insert(newArr[in_table]->getTail(), current->getData(), current->i);
                    current->setData(nullptr);
                    current = current->next;
                }
               // arr[i]->setTail(arr[i]->getHead());
                //arr[i]->getHead()->setNext(nullptr);
                delete arr[i];
            }
            delete [] arr;

            arr = newArr;
            tableSize = newTableSize;
        }


    }
};

#endif //WET2_CHAINEDHASH_H
