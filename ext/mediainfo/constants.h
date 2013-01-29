#ifndef GEM_MEDIAINFO_CONSTANTS_H
#define GEM_MEDIAINFO_CONSTANTS_H

extern VALUE cMediaInfo_mStream;
extern VALUE cMediaInfo_mInfo;
extern VALUE cMediaInfo_mInfoOption;
extern VALUE cMediaInfo_mFileOption;

const char *mediainfo_stream_type_name(int stream_type);
void Init_mediainfo_constants(void);

#endif /*GEM_MEDIAINFO_CONSTANTS_H*/
