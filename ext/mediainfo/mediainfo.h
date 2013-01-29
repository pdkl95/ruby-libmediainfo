#ifndef GEM_MEDIAINFO_MEDIAINFO_H
#define GEM_MEDIAINFO_MEDIAINFO_H

extern VALUE cMediaInfo;

struct mediainfo {
    void *handle;
    VALUE path;
};
typedef struct mediainfo mediainfo_t;

#define GET_MI(mi, selfptr)                            \
    mediainfo_t *mi;                                   \
    Data_Get_Struct(selfptr, mediainfo_t, mi);

#define UNPACK_MI GET_MI(mi, self)

void Init_mediainfo_Mediainfo(void);

#endif /*GEM_MEDIAINFO_MEDIAINFO_H*/
