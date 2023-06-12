//
// Created by Gharam on 6/11/2020.
//

#ifndef WET2_SONG_H
#define WET2_SONG_H

class Song {
public:
    int songId;
    int ArtistId;
    int numOfStreams;

    Song(int songId,int artistId):songId(songId), ArtistId(artistId),numOfStreams(0){};
    ~Song()= default;

    
};

bool operator<(const Song& song1, const Song& song2) {
    if(song1.songId==song2.songId){
        return ((song1.numOfStreams)>(song2.numOfStreams));
    }
    return (song1.songId < song2.songId);
}

bool operator==(const Song& song1, const Song& song2) {
    return (song1.songId==song2.songId);
}

bool operator>(const Song& song1, const Song& song2) {
    return (song2 < song1);
}
#endif //WET2_SONG_H
