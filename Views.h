//
// Created by basel on 6/12/2020.
//


#ifndef WET2_VIEWS_H
#define WET2_VIEWS_H

class Streams {
public:
    int songId;
    int ArtistId;
    int numOfStreams;

    Streams(int songId,int artistId):songId(songId), ArtistId(artistId),numOfStreams(0){};
    ~Streams()= default;

};

bool operator<(const Streams& view1, const Streams& view2) {
    if(view1.numOfStreams == view2.numOfStreams){
        if(view1.ArtistId == view2.ArtistId) {
            return (view1.songId > view2.songId);
        }
        return view1.ArtistId > view2.ArtistId;
    }
    return (view1.numOfStreams < view2.numOfStreams);
}

bool operator==(const Streams& view1, const Streams& view2) {
    return (view1.songId == view2.songId) && (view1.ArtistId == view2.ArtistId);
}

bool operator>(const Streams& view1, const Streams& view2) {
    return (view2 < view1);
}


#endif //WET2_VIEWS_H
