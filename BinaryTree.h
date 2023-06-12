//
// Created by basel on 6/10/2020.
//

#ifndef WET2_BINARYTREE_H
#define WET2_BINARYTREE_H


#include "library2.h"
#include <cstdio>
#include <cstdlib>


template <class Element >
class BinaryTreeNode{
public:
    Element* data;
    int height;
    BinaryTreeNode<Element>* left;
    BinaryTreeNode<Element>* right;
    BinaryTreeNode<Element>* perant;
    explicit BinaryTreeNode<Element>(Element* data,int height=0):data(nullptr), height(height), left(nullptr),
                                                              right(nullptr), perant(nullptr) {
        this->data=data;
    }

    void setRight(BinaryTreeNode* val) {
        right = val;
    }

    void setLeft(BinaryTreeNode* val) {
        left = val;
    }

    void setHeight(int val) {
        height = val;
    }

    void setParent(BinaryTreeNode* val) {
        perant = val;
    }

    int getHeight() {
        if(this == nullptr)
            return -1;
        return height;
    }

    void setData(Element* val) {
        data = val;
    }

    BinaryTreeNode<Element>* getRight() {
        return right;
    }
    BinaryTreeNode<Element>* getLeft() {
        return left;
    }
    BinaryTreeNode<Element>* getParent() {
        return perant;
    }
    Element* getData() {
        return data;
    }
    ~BinaryTreeNode<Element>() {
        delete data;
    }
    BinaryTreeNode<Element>* findInOrder() {
        BinaryTreeNode<Element> *tmp = this;
        tmp = tmp->getRight();
        if (tmp != nullptr) {
            while (tmp->getLeft() != nullptr)
                tmp = tmp->getLeft();
            return tmp;
        }
        tmp = this;
        BinaryTreeNode<Element>* tmp_parent = tmp->getParent();
        while(tmp_parent != nullptr) {
            if(tmp_parent->getLeft() == tmp)
                return tmp_parent;
            tmp = tmp_parent;
            tmp_parent = tmp->getParent();
        }
        return nullptr;
    }
};


template <class Element>
class BinaryTree{
public:
    BinaryTreeNode<Element> * root;
    BinaryTreeNode<Element> * max;

    explicit BinaryTree<Element>(): root(nullptr), max(nullptr) {
    }

    ~BinaryTree<Element>() {
        destructorHelper(root);
    }

    void destructorHelper(BinaryTreeNode<Element> *node){
        if (node == nullptr) {
            return;
        }
        destructorHelper(node->getRight());
        destructorHelper(node->getLeft());
        delete node;
        max = nullptr;
    }

    StatusType insertToTree_aux(BinaryTreeNode<Element>* root, Element* data) {
        if(!root) {
            BinaryTreeNode<Element>* newBinaryTreeNode=new BinaryTreeNode<Element>(data,1);
            this->root = newBinaryTreeNode;
            return SUCCESS;
        }
        if(*data == *(root->data))
            return FAILURE;
        StatusType result;
        if (*data > *(root->getData()) && root->getRight() != nullptr) {
            result = insertToTree(root->right, data);
            return result;
        } else if (*data > *(root->getData()) && root->getRight() == nullptr) {
            BinaryTreeNode<Element>* newBinaryTreeNode = new BinaryTreeNode<Element>(data, 1);
            root->setRight ( newBinaryTreeNode);
            newBinaryTreeNode->setParent(root) ;
        } else if ((*data < *(root->getData()) || *data == *(root->getData())) && root->getLeft() != nullptr) {
            result = insertToTree(root->getLeft(), data);
            return result;
        } else if ((*data < *(root->getData()) || *data == *(root->getData())) && root->getLeft() == nullptr) {
            BinaryTreeNode<Element>* newBinaryTreeNode = new BinaryTreeNode<Element>(data, 1);
            root->setLeft (newBinaryTreeNode);
            newBinaryTreeNode->setParent( root);
        }

        return SUCCESS;
    }

    StatusType insertToTree(BinaryTreeNode<Element>* root, Element* data) {
        StatusType result = insertToTree_aux(root, data);
        this->updateMax();
        return result;
    }



    void removeFromTree_aux(BinaryTreeNode<Element>* node) {
        if (node == nullptr)
            return;
        BinaryTreeNode<Element>* parent = node->perant;
        if (node->getLeft() == nullptr && node->getRight() == nullptr) {
            if (!parent) {
                delete node;
                this->root = nullptr;
            } else {
                if (parent->getLeft() == node) {
                    delete node;
                    parent->setLeft(nullptr);
                } else {
                    if (parent->getRight() == node) {
                        delete node;
                        parent->setRight(nullptr);
                    }
                }
            }
        }  else if (node->getLeft() != nullptr && !(node->getRight())) {
            if (parent== nullptr) {
                this->root = node->getLeft();
                node->getLeft()->setParent( nullptr);
                delete (node);
            } else {
                if (parent->getLeft() == node) {
                    parent->setLeft( node->left);
                    node->getLeft()->setParent( parent);
                    delete (node);
                } else {
                    if (parent->getRight() == node) {
                        parent->setRight( node->getLeft());
                        node->getLeft()->setParent( parent);
                        delete (node);
                    }
                }
            }
        } else if (node->getLeft() == nullptr && node->getRight() != nullptr) {
            if (!parent) {
                this->root = node->getRight();
                node->getRight()->setParent( nullptr);
                delete (node);
            } else {
                if (parent->getLeft() == node) {
                    parent->setLeft( node->right);
                    node->getRight()->setParent( parent);
                    delete (node);
                } else {
                    if (parent->getRight() == node) {
                        parent->setRight( node->right);
                        node->getRight()->setParent(parent);
                        delete (node);
                    }
                }
            }
        }
        else {
            BinaryTreeNode<Element>* node_after = node->findInOrder();
            Element* data = node_after->getData();
            node_after->setData(node->getData());
            node->setData(data) ;
            removeFromTree_aux(node_after);
            return;
        }

    }

    StatusType removeFromTree(BinaryTreeNode<Element>* root, Element *data) {
        if (this == nullptr || root == nullptr) {
            return FAILURE;
        }
        BinaryTreeNode<Element>* to_remove = root;
        while (to_remove != nullptr) {
            if (*(to_remove->getData()) == *data) {
                break;
            }
            if (*(to_remove->getData()) > *data)
                to_remove = to_remove->left;
            else to_remove = to_remove->right;
        }
        removeFromTree_aux(to_remove);
        updateMax();
        return SUCCESS;
    }

    BinaryTreeNode<Element>* insertAllSongs_aux(int start, int end) {
        if (start > end)
            return nullptr;
        int mid = (start + end) / 2;
        Element*root_value = new Element(mid);
        BinaryTreeNode<Element> *tmp_root = new BinaryTreeNode<Element>(root_value);
        tmp_root->setLeft(insertAllSongs_aux(start, mid - 1));
        tmp_root->setRight(insertAllSongs_aux(mid + 1, end));
        int heightLeft = 0;
        int heightRight = 0;
        if (tmp_root->getRight() != nullptr) {
            tmp_root->getRight()->setParent(tmp_root);
            heightRight = tmp_root->getRight()->getHeight();
        }
        if (tmp_root->getLeft() != nullptr) {
            tmp_root->getLeft()->setParent(tmp_root);
            heightRight = tmp_root->getLeft()->getHeight();
        }
        tmp_root->setHeight(getMaximum(heightLeft, heightRight));
        return tmp_root;

    }

    int getMaximum(int height1, int height2) {
        if(height1 > height2)
            return height1;
        return height2;
    }

    Element* findInTree(Element* data){
        if (root == nullptr)
            return nullptr;
        BinaryTreeNode<Element>* find= root;
        while (find != nullptr) {
            if (*(find->getData()) == *data)
                return find->getData();
            if (*(find->getData()) > *data)
                find= find->getLeft();
            else find = find->getRight();
        }
        return nullptr;
    }

    BinaryTreeNode<Element>* getRoot() {
        return root;
    }

    BinaryTreeNode<Element>* getMax() {
        return max;
    }

    void  updateMax() {
        BinaryTreeNode<Element> *tmp;
        tmp = this->root;
        if(tmp == nullptr) {
            this->max = nullptr;
            return;
        }
        while(tmp->getRight() != nullptr)
            tmp = tmp->getRight();
        this->max = tmp;
    }

};



#endif //WET2_BINARYTREE_H
