//
// Created by basel on 6/12/2020.
//

#ifndef WET2_MUSICMANAGER2_H
#define WET2_MUSICMANAGER2_H


#include "Artist.h"
#include "RankTree.h"
#include "Views.h"
#include "ChainedHash.h"

class MusicManager2 {
    hashTable<Artist>* artists;
    RankTree<Streams>* rankTree;
public:
    MusicManager2();

    ~MusicManager2();

    StatusType AddDataCenter(int artistID);

    StatusType RemoveDataCenter(int artistID);

    StatusType AddToSongDataCenter(int artistID, int songID);

    StatusType RemoveSongDataCenter(int artistID, int songID);

    StatusType AddToSongCountDataCenter(int artistID, int songID, int count);

    StatusType GetBestSongCenter(int artistID, int* songID);

    StatusType RecommendedDataCenter(int rank, int *artistId, int *songId);


};

MusicManager2::MusicManager2() {
    artists = new hashTable<Artist>();
    rankTree = new RankTree<Streams>();
}

MusicManager2::~MusicManager2() {
    delete artists;
    delete rankTree;
}

StatusType MusicManager2::AddDataCenter(int artistID) {
    //Artist* to_add = new Artist(artistID);
    if(artists->insert(artistID) == 0)
        return FAILURE;
    return SUCCESS;
}

StatusType MusicManager2::RemoveDataCenter(int artistID) {
    Node<Artist>* to_find = artists->Member(artistID);
    if(to_find == nullptr || to_find->data->songsTree->root != nullptr)
        return FAILURE;
    artists->Delete(artistID, to_find);
    return SUCCESS;
}

StatusType MusicManager2::AddToSongDataCenter(int artistID, int songID) {
    Song* song_to_add = new Song(songID, artistID);
    Node<Artist>* add_to_trees = artists->Member(artistID);
    // if exists
    if(add_to_trees == nullptr) {
        delete song_to_add;
        return FAILURE;
    }
    Song* song_to_find = add_to_trees->getData()->songsTree->findInTree(song_to_add);
    if(song_to_find != nullptr) {
        delete song_to_add;
        return FAILURE;
    }

    // add song to rank tree
    Streams* rank_to_add = new Streams(songID, artistID);
    rankTree->insertToTree(rankTree->root, rank_to_add);

    // add song to hash table
    Streams* stream_to_add = new Streams(songID, artistID);
    add_to_trees->getData()->songsTree->insertToTree(add_to_trees->getData()->songsTree->root, song_to_add);
    add_to_trees->getData()->viewsTree->insertToTree(add_to_trees->getData()->viewsTree->root, stream_to_add);
    return SUCCESS;
}

StatusType MusicManager2::RemoveSongDataCenter(int artistID, int songID){
    Song* song_to_remove = new Song(songID, artistID);
    Node<Artist>* findArtist = artists->Member(artistID);
    // if exists
    if(findArtist == nullptr) {
        delete song_to_remove;
        return FAILURE;
    }
    Song* song_to_find = findArtist->getData()->songsTree->findInTree(song_to_remove);
    if(song_to_find == nullptr) {
        delete song_to_remove;
        return FAILURE;
    }

    // remove song
    Streams* rank_to_remove = new Streams(songID, artistID);
    rank_to_remove->numOfStreams = song_to_find->numOfStreams;
    rankTree->removeFromTree(rankTree->root, rank_to_remove);
    findArtist->getData()->viewsTree->removeFromTree(findArtist->getData()->viewsTree->getRoot(), rank_to_remove);
    findArtist->getData()->songsTree->removeFromTree(findArtist->getData()->songsTree->getRoot(), song_to_remove);
    delete rank_to_remove;
    delete song_to_remove;
    return SUCCESS;
}


 StatusType MusicManager2::AddToSongCountDataCenter(int artistID, int songID, int count) {
    Song* song_to_add = new Song(songID, artistID);
    Node<Artist>* add_to_trees = artists->Member(artistID);
    // if exists
    if(add_to_trees == nullptr) {
        delete song_to_add;
        return FAILURE;
    }
    Song* song_to_find = add_to_trees->getData()->songsTree->findInTree(song_to_add);
    if(song_to_find == nullptr) {
        delete song_to_add;
        return FAILURE;
    }

    Streams* rank_to_add = new Streams(songID, artistID);
    Streams* stream_to_add = new Streams(songID, artistID);
    song_to_add->numOfStreams = count + song_to_find->numOfStreams;
    rank_to_add->numOfStreams = count + song_to_find->numOfStreams;
    stream_to_add->numOfStreams = count + song_to_find->numOfStreams;

    // remove song with old count
    RemoveSongDataCenter(artistID, songID);

    // add count to rank tree
    rankTree->insertToTree(rankTree->root, rank_to_add);

    // add count to hash table
    add_to_trees->getData()->songsTree->insertToTree(add_to_trees->getData()->songsTree->root, song_to_add);
    add_to_trees->getData()->viewsTree->insertToTree(add_to_trees->getData()->viewsTree->root, stream_to_add);
    return SUCCESS;
}


StatusType MusicManager2::GetBestSongCenter(int artistID, int* songID) {
    Node<Artist>* findArtist = artists->Member(artistID);
    if(!findArtist||findArtist->getData()->songsTree->getRoot()== nullptr){
        return FAILURE;
    }
    *songID=findArtist->getData()->viewsTree->getMax()->getData()->songId;
    return SUCCESS;
}

StatusType MusicManager2::RecommendedDataCenter(int rank, int *artistId, int *songId) {
    RankTreeNode<Streams>* rankedSong = rankTree->Select(rank);
    if(rankedSong == nullptr || rankTree->root->getSubTreesCount() < rank)
        return FAILURE;
    *artistId = rankedSong->getData()->ArtistId;
    *songId = rankedSong->getData()->songId;
    return SUCCESS;
}


#endif //WET2_MUSICMANAGER2_H
