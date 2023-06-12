//
// Created by basel on 6/10/2020.
//

#include "library2.h"
#include "MusicManager2.h"

void *Init() {
    MusicManager2 *DS = new MusicManager2();
    return (void*)DS;
}

StatusType AddArtist(void *DS, int artistID) {
    if(DS == nullptr || artistID <= 0)
        return INVALID_INPUT;
    return ((MusicManager2*)DS)->AddDataCenter(artistID);
}

StatusType RemoveArtist(void *DS, int artistID) {
    if(DS == nullptr || artistID <= 0)
        return INVALID_INPUT;
    return ((MusicManager2*)DS)->RemoveDataCenter(artistID);
}

StatusType AddSong(void *DS, int artistID, int songID) {
    if(DS == nullptr || artistID <= 0 || songID <= 0)
        return INVALID_INPUT;
    return ((MusicManager2*)DS)->AddToSongDataCenter(artistID, songID);
}

StatusType RemoveSong(void *DS, int artistID, int songID) {
    if(DS == nullptr || artistID <= 0 || songID <= 0)
        return INVALID_INPUT;
    return ((MusicManager2*)DS)->RemoveSongDataCenter(artistID, songID);
}

StatusType AddToSongCount(void *DS, int artistID, int songID, int count) {
    if(DS == nullptr || artistID <= 0 || songID <= 0 || count <= 0)
        return INVALID_INPUT;
    return ((MusicManager2*)DS)->AddToSongCountDataCenter(artistID, songID, count);
}

StatusType GetArtistBestSong(void *DS, int artistID, int *songId) {
    if (DS == nullptr || artistID <= 0 || songId == NULL)
        return INVALID_INPUT;
    return ((MusicManager2*)DS)->GetBestSongCenter(artistID, songId);
}

StatusType GetRecommendedSongInPlace(void *DS, int rank, int *artistId, int *songId) {
    if(DS == nullptr || artistId == nullptr || songId == nullptr || rank <= 0)
        return INVALID_INPUT;
    return ((MusicManager2*)DS)->RecommendedDataCenter(rank, artistId, songId);
}

void Quit(void** DS) {
    if(DS != nullptr && *DS != nullptr) {
        delete (MusicManager2*)*DS;
        *DS = NULL;
    }
}