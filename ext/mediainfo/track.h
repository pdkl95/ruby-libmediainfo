#ifndef GEM_MEDIAINFO_TRACK_H
#define GEM_MEDIAINFO_TRACK_H

extern VALUE cMediaInfo_cTrack;

struct mediainfo_track {
    VALUE mediainfo;
    VALUE stream_type;

    VALUE id;
    int id_int;
    int num_fields;
    VALUE fields;
};
typedef struct mediainfo_track mediainfo_track_t;

#define GET_MEDIAINFO_TRACK(track, selfptr)     \
    GET_CLASS_STRUCT(mediainfo_track_t, track, selfptr)

#define UNPACK_TRACK GET_MEDIAINFO_TRACK(track, self)

void Init_mediainfo_MediaInfo_Track(void);

#endif /*GEM_MEDIAINFO_TRACK_H*/
