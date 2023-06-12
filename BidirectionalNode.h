//
// Created by basel on 5/13/2020.
//

#ifndef WET1_BIDIRECTIONALNODE_H
#define WET1_BIDIRECTIONALNODE_H

template <class Element>
class Node {
public:
    int i;
    Element* data;
    Node<Element> *prev;
    Node<Element> *next;

public:
    Node();
    ~Node();
    Element* getData();
    void setData(Element* element);
    Node<Element>* getNext();
    Node<Element>* getPrev();
    void setNext(Node<Element> *val) {
        this->next = val;
    }
    void setPrev(Node<Element> *val) {
        this->prev = val;
    }
};

template <class Element>
Node<Element>::Node():i(-1),data(nullptr), prev(nullptr), next(nullptr) {
}

//template<class Element>
//Node<Element>::Node(Element& data): data(data), prev(nullptr), next(nullptr) {}

template<class Element>
Node<Element>::~Node() {
    if(data != nullptr)
        delete data;
    prev = nullptr;
    next = nullptr;
}

template<class Element>
Element *Node<Element>::getData() {
    if(this == nullptr)
        return nullptr;
    return this->data;
}


template<class Element>
Node<Element> *Node<Element>::getNext() {
    if(this == nullptr)
        return nullptr;
    return this->next;
}


template<class Element>
Node<Element> *Node<Element>::getPrev() {
    if(this == nullptr)
        return nullptr;
    return this->prev;
}

template<class Element>
void Node<Element>::setData(Element *element) {
    this->data = element;
}

template <class Element>
class BidirectionalNode {
    Node<Element> *head;
    Node<Element> *tail;
public:
    explicit BidirectionalNode();
    ~BidirectionalNode();
    Node<Element>* insert(Node<Element>* place, Element* element, int i);
    void remove(Node<Element>* to_remove);
    Node<Element>* getFirst();
    Node<Element>* getHead();
    Node<Element>* getTail();
    void setTail(Node<Element>* node);
    Element* getData();

};

template<class Element>
BidirectionalNode<Element>::BidirectionalNode() {
    Node<Element>* dummyNode = new Node<Element>();
    head = dummyNode;
    tail = dummyNode;
}

template<class Element>
BidirectionalNode<Element>::~BidirectionalNode() {
    while(head != tail) {
        remove(head->getNext());
    }
    delete head;
}

template<class Element>
Node<Element>* BidirectionalNode<Element>::insert(Node<Element> *place, Element* element, int i) {
    Node<Element> *tmp;
    if (place == nullptr)
        return nullptr;
    tmp = new Node<Element>();
    tmp->i = i;
    tmp->setData(element);
    tmp->setNext(place->getNext());
    tmp->setPrev(place);
    if(tmp->getNext() != nullptr)
        tmp->getNext()->setPrev(tmp);
    else tail = tmp;
    place->setNext(tmp);
    return tmp;
}

template<class Element>
void BidirectionalNode<Element>::remove(Node<Element> *to_remove) {
    if(to_remove == nullptr)
        return;
    if(to_remove->getNext() != nullptr)
        to_remove->getNext()->setPrev(to_remove->getPrev());
    else tail = to_remove->getPrev();
    to_remove->getPrev()->setNext(to_remove->getNext());
    delete to_remove;
}

template<class Element>
Node<Element> *BidirectionalNode<Element>::getFirst() {
    return head->getNext();
}

template<class Element>
Node<Element> *BidirectionalNode<Element>::getHead() {
    return head;
}

template<class Element>
Node<Element> *BidirectionalNode<Element>::getTail() {
    return tail;
}

template<class Element>
void BidirectionalNode<Element>::setTail(Node<Element> *node) {
    this->tail = node;
}


#endif //WET1_BIDIRECTIONALNODE_H
