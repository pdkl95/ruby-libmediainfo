#ifndef GEM_MEDIAINFO_STREAM_TYPE_H
#define GEM_MEDIAINFO_STREAM_TYPE_H

const char *mediainfo_stream_type_to_name(int stream_type);

extern VALUE cMediaInfo_cStreamType;

struct mediainfo_stream_type {
    VALUE mediainfo;

    int type;
    const char *cname;
    VALUE name;
    VALUE tracks;
    size_t num_tracks;
};
typedef struct mediainfo_stream_type mediainfo_stream_type_t;

#define GET_MEDIAINFO_STREAM_TYPE(st, selfptr)     \
    GET_CLASS_STRUCT(mediainfo_stream_type_t, st, selfptr)

#define UNPACK_STREAM_TYPE GET_MEDIAINFO_STREAM_TYPE(st, self)

void Init_mediainfo_MediaInfo_StreamType(void);

#endif /*GEM_MEDIAINFO_STREAM_TYPE_H*/
