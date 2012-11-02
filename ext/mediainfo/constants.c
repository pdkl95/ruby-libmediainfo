#include "common.h"

VALUE cMediaInfo_mStream;
VALUE cMediaInfo_mInfo;
VALUE cMediaInfo_mInfoOption;
VALUE cMediaInfo_mFileOption;

void init_mediainfo_constants()
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
