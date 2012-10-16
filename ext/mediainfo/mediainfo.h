#ifndef GEM_MEDIAINFO_H
#define GEM_MEDIAINFO_H

extern VALUE cMediaInfo;
extern VALUE cMediaInfo_mStream;
extern VALUE cMediaInfo_mInfo;
extern VALUE cMediaInfo_mInfoOpt;
extern VALUE cMediaInfo_mFileOpt;


struct media_info {
    void *handle;
};
typedef struct media_info mi_t;

#define UNPACK_MI                               \
    mi_t *mi;                                   \
    Data_Get_Struct(self, mi_t, mi);


#endif /*GEM_MEDIAINFO_H*/
