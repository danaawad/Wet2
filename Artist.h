//
// Created by Gharam on 5/8/2020.
//

#ifndef UNTITLED_ARTIST_H
#define UNTITLED_ARTIST_H


#include "BidirectionalNode.h"
#include "BinaryTree.h"
#include "Song.h"
#include "Views.h"

class Artist{
public:
    unsigned int id;
    BinaryTree<Song>* songsTree;
    BinaryTree<Streams>* viewsTree;

    Artist(unsigned int id):id(id), songsTree(new BinaryTree<Song>()), viewsTree(new BinaryTree<Streams>()) {
    }

    ~Artist() {
        delete songsTree;
        delete viewsTree;
    }


};


#endif //UNTITLED_ARTIST_H
