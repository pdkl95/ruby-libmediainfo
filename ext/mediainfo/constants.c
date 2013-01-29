#include "common.h"
#include "constants.h"
#include "mediainfo.h"

VALUE cMediaInfo_mStream;
VALUE cMediaInfo_mInfo;
VALUE cMediaInfo_mInfoOption;
VALUE cMediaInfo_mFileOption;

const char *
mediainfo_stream_type_name(int stream_type)
{
    switch(stream_type) {
    case MediaInfo_Stream_General:  return "general";
    case MediaInfo_Stream_Video:    return "video";
    case MediaInfo_Stream_Audio:    return "audio";
    case MediaInfo_Stream_Text:     return "text";
    case MediaInfo_Stream_Chapters: return "chapters";
    case MediaInfo_Stream_Image:    return "image";
    case MediaInfo_Stream_Menu:     return "menu";
    default:                        return "unknown";
    }
}

void
Init_mediainfo_constants(void)
{
#define MOD(name)                                                       \
    JOIN(cMediaInfo_m,name) = rb_define_module_under(cMediaInfo, Q(name))

    MOD(Stream);
    MOD(Info);
    MOD(InfoOption);
    MOD(FileOption);
#undef MOD

#define MI_CONST(cat, name)                     \
    JOIN(JOIN(MediaInfo_,cat),JOIN(_,name))

#define C(cat, name)                                                    \
    rb_define_const( JOIN(cMediaInfo_m,cat),                            \
                     Q(name),                                           \
                     INT2NUM(MI_CONST(cat,name)))

    C(Stream,General);
    C(Stream,Video);
    C(Stream,Audio);
    C(Stream,Text);
    C(Stream,Chapters);
    C(Stream,Image);
    C(Stream,Menu);
    C(Stream,Max);

    C(Info,Name);
    C(Info,Text);
    C(Info,Measure);
    C(Info,Options);
    C(Info,Name_Text);
    C(Info,Measure_Text);
    C(Info,Info);
    C(Info,HowTo);
    C(Info,Max);

    C(InfoOption,ShowInInform);
    C(InfoOption,Reserved);
    C(InfoOption,ShowInSupported);
    C(InfoOption,TypeOfValue);
    C(InfoOption,Max);

    C(FileOption,Nothing);
    C(FileOption,NoRecursive);
    C(FileOption,CloseAll);
    C(FileOption,Max);
#undef C
#undef MI_CONST_NAME
}
