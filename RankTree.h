//
// Created by basel on 6/10/2020.
//

#ifndef WET2_RANKTREE_H
#define WET2_RANKTREE_H

#include "library2.h"
#include <cstdio>
#include <cstdlib>

template <class Element >
class RankTreeNode{
public:
    Element* data;
    int height;
    RankTreeNode<Element>* left;
    RankTreeNode<Element>* right;
    RankTreeNode<Element>* perant;
    int subTreesCount;

    explicit RankTreeNode<Element>(Element* data,int height=0):data(nullptr), height(height), left(nullptr),
                                                              right(nullptr), perant(nullptr), subTreesCount(1) {
        this->data=data;
    }

    void setSubTreesCount(int count) {
        subTreesCount = count;
    }

    int getSubTreesCount() {
        if(this == nullptr)
            return 0;
        return subTreesCount;
    }

    void setRight(RankTreeNode* val) {
        right = val;
    }

    void setLeft(RankTreeNode* val) {
        left = val;
    }

    void setHeight(int val) {
        height = val;
    }

    void setParent(RankTreeNode* val) {
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

    RankTreeNode<Element>* getRight() {
        return right;
    }
    RankTreeNode<Element>* getLeft() {
        return left;
    }
    RankTreeNode<Element>* getParent() {
        return perant;
    }
    Element* getData() {
        return data;
    }
    ~RankTreeNode<Element>() {
        delete data;
    }
    RankTreeNode<Element>* findInOrder() {
        RankTreeNode<Element> *tmp = this;
        tmp = tmp->getRight();
        if (tmp != nullptr) {
            while (tmp->getLeft() != nullptr)
                tmp = tmp->getLeft();
            return tmp;
        }
        tmp = this;
        RankTreeNode<Element>* tmp_parent = tmp->getParent();
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
class RankTree {
public:
    RankTreeNode<Element> *root;
    RankTreeNode<Element> *max;

    explicit RankTree<Element>() : root(nullptr), max(nullptr) {
    }

    ~RankTree<Element>() {
        destructorHelper(root);
    }

    void destructorHelper(RankTreeNode<Element> *node) {
        if (node == nullptr) {
            return;
        }
        destructorHelper(node->getRight());
        destructorHelper(node->getLeft());
        delete node;
        max = nullptr;
    }

    void updateRanks(RankTreeNode<Element> *node) {
        while (node != nullptr) {
            node->setSubTreesCount(node->getSubTreesCount() + 1);
            node = node->getParent();
        }
    }


    StatusType insertToTree_aux(RankTreeNode<Element> *root, Element *data) {
        if (!root) {
            RankTreeNode<Element> *newRankTreeNode = new RankTreeNode<Element>(data, 1);
            this->root = newRankTreeNode;
            return SUCCESS;
        }
        if (*data == *(root->data))
            return FAILURE;
        StatusType result;
        if (*data > *(root->getData()) && root->getRight() != nullptr) {
            result = insertToTree(root->right, data);
            return result;
        } else if (*data > *(root->getData()) && root->getRight() == nullptr) {
            RankTreeNode<Element> *newRankTreeNode = new RankTreeNode<Element>(data, 1);
            root->setRight(newRankTreeNode);
            newRankTreeNode->setParent(root);
            //updateRanks(root);
        } else if ((*data < *(root->getData()) || *data == *(root->getData())) && root->getLeft() != nullptr) {
            result = insertToTree(root->getLeft(), data);
            return result;
        } else if ((*data < *(root->getData()) || *data == *(root->getData())) && root->getLeft() == nullptr) {
            RankTreeNode<Element> *newRankTreeNode = new RankTreeNode<Element>(data, 1);
            root->setLeft(newRankTreeNode);
            newRankTreeNode->setParent(root);
         //   updateRanks(root);
        }
        Rotate(root);
        return SUCCESS;
    }

    StatusType insertToTree(RankTreeNode<Element> *root, Element *data) {
        Element *is_exists = findInTree(data);
        if (is_exists != nullptr)
            return FAILURE;
        StatusType result = insertToTree_aux(root, data);
        this->updateMax();
        return result;
    }

    void Rotate(RankTreeNode<Element> *root) {
        if (!root) {
            return;
        }
        root->height = getMaxHeight(root->getLeft(), root->getRight());
        root->subTreesCount = root->getLeft()->getSubTreesCount() + root->getRight()->getSubTreesCount() + 1;
        int balancefactor = getBalanceFactor(root);
        int balancefactor_right = getBalanceFactor(root->getRight());
        int balancefactor_left = getBalanceFactor(root->getLeft());
        if (balancefactor == 2) {
            if (balancefactor_left >= 0) {
                LLrotate(root);
                Rotate(root->getParent());
                return;
            }
            RRrotate(root->getLeft());
            LLrotate(root);
            Rotate(root->getParent());
            return;
        }
        if (balancefactor == -2) {
            if (balancefactor_right == 1) {
                LLrotate(root->getRight());
                RRrotate(root);
                Rotate(root->getParent());
                return;
            }
            RRrotate(root);
            Rotate(root->getParent());
            return;
        }
        Rotate(root->getParent());
    }

    int getMaxHeight(RankTreeNode<Element> *left, RankTreeNode<Element> *right) {
        int leftHeight, RightHeight;
        if (left == nullptr)
            leftHeight = 0;
        else leftHeight = left->height;

        if (right == nullptr)
            RightHeight = 0;
        else RightHeight = right->height;

        if (RightHeight > leftHeight)
            return RightHeight + 1;
        return leftHeight + 1;
    }

    int getBalanceFactor(RankTreeNode<Element> *root) {
        if (root == nullptr)
            return 0;
        int leftHeight;
        int rightHeight;
        if (root->getLeft() == nullptr)
            leftHeight = 0;
        else leftHeight = root->getLeft()->height;

        if (root->getRight() == nullptr)
            rightHeight = 0;
        else rightHeight = root->getRight()->height;
        return leftHeight - rightHeight;
    }

    void LLrotate(RankTreeNode<Element> *tree) {
        if (!tree) {
            return;
        }
        RankTreeNode<Element> *wanted_node = tree->getLeft();
        RankTreeNode<Element> *grandparent = tree->getParent();
        tree->setLeft(wanted_node->getRight());
        if (wanted_node->getRight() != nullptr) {
            wanted_node->getRight()->setParent(tree);
        }
        wanted_node->setRight(tree);
        tree->setParent(wanted_node);
        if (grandparent != nullptr) {
            if (grandparent->getRight() == tree) {
                grandparent->setRight(wanted_node);
            } else {
                grandparent->setLeft(wanted_node);
            }
        }
        wanted_node->setParent(grandparent);
        wanted_node->getRight()->height = getMaxHeight(wanted_node->getRight()->getLeft(),
                                                       wanted_node->getRight()->getRight());
        wanted_node->getRight()->subTreesCount = wanted_node->getRight()->getLeft()->getSubTreesCount()
                                                  + wanted_node->getRight()->getRight()->getSubTreesCount() + 1;
        wanted_node->height = getMaxHeight(wanted_node->getLeft(), wanted_node->getRight());
        wanted_node->subTreesCount = wanted_node->getLeft()->getSubTreesCount()
                                                 + wanted_node->getRight()->getSubTreesCount() + 1;
        if (!grandparent) {
            this->root = wanted_node;
            return;
        }
        grandparent->setHeight(getMaxHeight(grandparent->getLeft(), grandparent->getRight()));
        grandparent->subTreesCount = grandparent->getLeft()->getSubTreesCount()
                                     + grandparent->getRight()->getSubTreesCount() + 1;
    }


    void RRrotate(RankTreeNode<Element> *tree) {
        if (!tree) {
            return;
        }
        RankTreeNode<Element> *wanted_node = tree->getRight();
        RankTreeNode<Element> *grandparent = tree->getParent();
        tree->setRight(wanted_node->getLeft());
        if (wanted_node->getLeft() != nullptr) {
            wanted_node->getLeft()->setParent(tree);
        }
        wanted_node->setLeft(tree);
        tree->setParent(wanted_node);
        if (grandparent != nullptr) {
            if (grandparent->getRight() == tree) {
                grandparent->setRight(wanted_node);
            } else {
                grandparent->setLeft(wanted_node);
            }
        }
        wanted_node->setParent(grandparent);
        wanted_node->getLeft()->height = getMaxHeight(wanted_node->getLeft()->getLeft(),
                                                      wanted_node->getLeft()->getRight());
        wanted_node->getLeft()->subTreesCount = wanted_node->getLeft()->getLeft()->getSubTreesCount()
                                                 + wanted_node->getLeft()->getRight()->getSubTreesCount() + 1;
        wanted_node->height = getMaxHeight(wanted_node->getLeft(),
                                           wanted_node->getRight());
        wanted_node->subTreesCount = wanted_node->getLeft()->getSubTreesCount()
                                                + wanted_node->getRight()->getSubTreesCount() + 1;
        if (!grandparent) {
            this->root = wanted_node;
            return;
        }
        grandparent->height = getMaxHeight(grandparent->getLeft(), grandparent->getRight());
        grandparent->subTreesCount = grandparent->getLeft()->getSubTreesCount()
                                     + grandparent->getRight()->getSubTreesCount() + 1;
    }

    void removeFromTree_aux(RankTreeNode<Element> *node) {
        if (node == nullptr)
            return;
        RankTreeNode<Element> *parent = node->perant;
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
        } else if (node->getLeft() != nullptr && !(node->getRight())) {
            if (parent == nullptr) {
                this->root = node->getLeft();
                node->getLeft()->setParent(nullptr);
                delete (node);
            } else {
                if (parent->getLeft() == node) {
                    parent->setLeft(node->left);
                    node->getLeft()->setParent(parent);
                    delete (node);
                } else {
                    if (parent->getRight() == node) {
                        parent->setRight(node->getLeft());
                        node->getLeft()->setParent(parent);
                        delete (node);
                    }
                }
            }
        } else if (node->getLeft() == nullptr && node->getRight() != nullptr) {
            if (!parent) {
                this->root = node->getRight();
                node->getRight()->setParent(nullptr);
                delete (node);
            } else {
                if (parent->getLeft() == node) {
                    parent->setLeft(node->right);
                    node->getRight()->setParent(parent);
                    delete (node);
                } else {
                    if (parent->getRight() == node) {
                        parent->setRight(node->right);
                        node->getRight()->setParent(parent);
                        delete (node);
                    }
                }
            }
        } else {
            RankTreeNode<Element> *node_after = node->findInOrder();
            Element *data = node_after->getData();
            node_after->setData(node->getData());
            node->setData(data);
            removeFromTree_aux(node_after);
            return;
        }
        Rotate(parent);
    }

    StatusType removeFromTree(RankTreeNode<Element> *root, Element *data) {
        if (this == nullptr || root == nullptr) {
            return FAILURE;
        }
        RankTreeNode<Element> *to_remove = root;
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


    Element *findInTree(Element *data) {
        if (root == nullptr)
            return nullptr;
        RankTreeNode<Element> *find = root;
        while (find != nullptr) {
            if (*(find->getData()) == *data)
                return find->getData();
            if (*(find->getData()) > *data)
                find = find->getLeft();
            else find = find->getRight();
        }
        return nullptr;
    }

    RankTreeNode<Element> *getRoot() {
        return root;
    }

    RankTreeNode<Element> *getMax() {
        return max;
    }

    void updateMax() {
        RankTreeNode<Element> *tmp;
        tmp = this->root;
        if (tmp == nullptr) {
            this->max = nullptr;
            return;
        }
        while (tmp->getRight() != nullptr)
            tmp = tmp->getRight();
        this->max = tmp;
    }

    int Rank(Element *data) {
        if (root == nullptr)
            return -1;
        int r = 0;
        RankTreeNode<Element> *find = root;
        while (find != nullptr) {
            if (*(find->getData()) == *data) {
                r = r + find->getLeft()->getSubTreesCount() + 1;
                break;
            }
            if (*(find->getData()) > *data)
                find = find->getLeft();
            else {
                r = r + find->getLeft()->getSubTreesCount() + 1;
                find = find->getRight();
            }
        }
        return r;
    }

    RankTreeNode<Element> *Select(int k) {
        if (root == nullptr)
            return nullptr;
        RankTreeNode<Element> *find = root;
        while (find != nullptr) {
            if (find->getRight()->getSubTreesCount() == (k - 1))
                return find;
            else if (find->getRight()->getSubTreesCount() > (k - 1))
                find = find->getRight();
            else {
                k = k - find->getRight()->getSubTreesCount() - 1;
                find = find->getLeft();
            }
        }
        return nullptr;
    }




};

#endif //WET2_RANKTREE_H
