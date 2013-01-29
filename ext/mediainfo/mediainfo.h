#ifndef GEM_MEDIAINFO_MEDIAINFO_H
#define GEM_MEDIAINFO_MEDIAINFO_H

extern VALUE cMediaInfo;

struct mediainfo {
    void *handle;

    VALUE path;

    VALUE stream_types;
    VALUE tracks;

    VALUE general;
    VALUE video;
    VALUE audio;
    VALUE text;
    VALUE chapters;
    VALUE image;
    VALUE menu;
};
typedef struct mediainfo mediainfo_t;

#define GET_MEDIAINFO(mi, selfptr)  \
    GET_CLASS_STRUCT(mediainfo_t, mi, selfptr)
#define UNPACK_MI GET_MEDIAINFO(mi, self)

VALUE mediainfo_get_i(VALUE self, VALUE stream_type, VALUE stream_id, VALUE field_id, VALUE request_type);
VALUE mediainfo_count_get(VALUE self, VALUE stream_type, VALUE stream_id);

void Init_mediainfo_Mediainfo(void);

#endif /*GEM_MEDIAINFO_MEDIAINFO_H*/
