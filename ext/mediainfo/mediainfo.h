#ifndef LIBMI_MEDIAINFO_H
#define LIBMI_MEDIAINFO_H

extern VALUE cMediaInfo;


struct media_info {
    void *handle;
};
typedef struct media_info mi_t;

#define UNPACK_MI                               \
    mi_t *mi;                                   \
    Data_Get_Struct(self, mi_t, mi);


#endif /*LIBMI_MEDIAINFO_H*/
