#ifndef LIBMI_MEDIAINFO_H
#define LIBMI_MEDIAINFO_H

#include <ruby.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>
#include <MediaInfoDLL/MediaInfoDLL.h>

#define Q(x) #x
#define JOIN_(a,b) a##b
#define JOIN(a,b)  JOIN_(a,b)

#if defined(UNICODE) || defined (_UNICODE)
#  define MINFO_EXT_UNI 1
#  define MINFO_EXT_FMT "%S"
#  define MINFO_EXT_LEN wcslen
#  define MINFO_EXT_ISDIGIT iswdigit
#  define MINFO_EXT_STRTOLL wcstoll
#  define RSTRING_TO_MINFO_FREE(x) xfree(x)
#else
#  undef MINFO_EXT_UNI
#  define MINFO_EXT_FMT "%s"
#  define MINFO_EXT_LEN strlen
#  define MINFO_EXT_ISDIGIT isdigit
#  define MINFO_EXT_STRTOLL strtoll
#  define RSTRING_TO_MINFO_FREE(x)
#endif

extern VALUE cMediaInfo;

struct media_info_api {
    void * (*new)();
    void   (*delete)(void *);
    size_t (*open)(void *, const MediaInfo_Char *);
    void   (*close)(void *);
    const MediaInfo_Char * (*inform)(void *, size_t);
    const MediaInfo_Char * (*geti)(void *, MediaInfo_stream_C, size_t, size_t, MediaInfo_info_C);
    const MediaInfo_Char * (*get)(void *, MediaInfo_stream_C, size_t, const MediaInfo_Char *, MediaInfo_info_C, MediaInfo_info_C);
    const MediaInfo_Char * (*option)(void *, const MediaInfo_Char *, const MediaInfo_Char *);
    size_t (*state_get)(void *);
    size_t (*count_get)(void *, MediaInfo_stream_C, size_t);
};
typedef struct media_info_api mi_api_t;

extern struct media_info_api cMediaInfoAPI;

struct media_info {
    void *handle;
    VALUE path;
};
typedef struct media_info mi_t;

#define GET_MI(mi, selfptr)                     \
    mi_t *mi;                                   \
    Data_Get_Struct(selfptr, mi_t, mi);

#define UNPACK_MI GET_MI(mi, self)


#endif /*LIBMI_MEDIAINFO_H*/
