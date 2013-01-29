#ifndef GEM_MEDIAINFO_API_H
#define GEM_MEDIAINFO_API_H

struct mediainfo_api {
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
typedef struct mediainfo_api mediainfo_api_t;

extern mediainfo_api_t cMediaInfoAPI;


#endif /*GEM_MEDIAINFO_API_H*/
