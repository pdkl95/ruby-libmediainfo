#include "mediainfo.h"

struct media_info_api cMediaInfoAPI;

VALUE cMediaInfo;
VALUE cMediaInfo_mStream;
VALUE cMediaInfo_mInfo;
VALUE cMediaInfo_mInfoOption;
VALUE cMediaInfo_mFileOption;

static VALUE
mediainfo_chars_to_rstring(const MediaInfo_Char *str)
{
    VALUE rstring;
    char *rstr;

#ifdef MINFO_EXT_UNI
    size_t len = wcstombs(NULL, str, 0);
    rstr = malloc(len + 1);
    if ((size_t)-1 == wcstombs(rstr, str, len)) {
        rb_raise(rb_eEncodingError, "could not convert given wchar_t* string to char* as used by libmediainfo");
    }
#else
    rstr = str;
#endif

    rstring = rb_str_new2(rstr);

#ifdef MINFO_EXT_UNI
    free(rstr);
#endif

    return rstring;
}

static MediaInfo_Char *
rstring_to_mediainfo_chars(VALUE rstring)
{
    MediaInfo_Char *mi_str;
    char *rstr = RSTRING_PTR(rstring);

#ifdef MINFO_EXT_UNI
    size_t len = mbstowcs(NULL, rstr, 0);
    mi_str = xmalloc(len + 1);
    if ((size_t)-1 == mbstowcs(mi_str, rstr, len)) {
        rb_raise(rb_eEncodingError, "could not convert given char* string to wchar_t* as needed by libmediainfo");
    }
    mi_str[len] = '\0';
#else
    mi_str = rstr;
#endif

    return mi_str;
}

static mi_t *
mi_t_alloc()
{
    mi_t *mi;
    mi = ALLOC(mi_t);
    mi->handle = cMediaInfoAPI.new();
    return mi;
}

static void
mi_t_free(mi_t *mi)
{
    cMediaInfoAPI.delete(mi->handle);
    xfree(mi);
}

static VALUE
mi_allocate(VALUE klass) {
    mi_t *mi = mi_t_alloc();
    if (!mi->handle) {
        rb_raise(rb_eRuntimeError, "MediaInfo_New() failed!");
    }
    return Data_Wrap_Struct(klass, NULL, mi_t_free, mi);
}

static VALUE
mi_open(int argc, VALUE *argv, VALUE self)
{
    VALUE file_path;
#define MAXLEN 4096
    MediaInfo_Char path[MAXLEN];
    size_t retval;
    UNPACK_MI;

    rb_scan_args(argc, argv, "10", &file_path);
    //path = rstring_to_mediainfo_chars(file_path);
    mbstowcs(path, RSTRING_PTR(file_path), MAXLEN);
#undef MAXLEN
    printf("mi_open: path = '" MINFO_EXT_FMT "'\n", path);fflush(stdout);
    if (!cMediaInfoAPI.open(mi->handle, path)) {
        rb_raise(rb_eIOError, "MediaInfo_Open() failed");
    }
    //RSTRING_TO_MINFO_FREE(path);
    return self;
}

static VALUE
mi_initialize(int argc, VALUE *argv, VALUE self)
{
    if (argc > 0 ) {
        return mi_open(argc, argv, self);
    } else {
        return self;
    }
}

static VALUE
mi_close(VALUE self)
{
    UNPACK_MI;
    cMediaInfoAPI.close(mi->handle);
    return self;
}

static const MediaInfo_Char *
mi_report_string(mi_t *mi)
{
    const MediaInfo_Char *str = cMediaInfoAPI.inform(mi->handle, 0);
    return str;
}

static VALUE
mi_inform(VALUE self)
{
    VALUE rstr;
    const MediaInfo_Char *str;
    UNPACK_MI;

    str = mi_report_string(mi);
    rstr = mediainfo_chars_to_rstring(str);
    return rstr;
}

static VALUE
mi_get_i(VALUE self, VALUE stream_type, VALUE stream_id, VALUE field_id, VALUE request_type)
{
    const MediaInfo_Char *mi_str;
    VALUE rstr;
    UNPACK_MI;

    mi_str = cMediaInfoAPI.geti(mi->handle,
                             NUM2INT(stream_type),
                             NUM2INT(stream_id),
                             NUM2INT(field_id),
                             NUM2INT(request_type));

    rstr = mediainfo_chars_to_rstring(mi_str);
    return rstr;
}

static VALUE
mi_get(VALUE self, VALUE stream_type, VALUE stream_id, VALUE field_name, VALUE request_type)
{
    VALUE rstr;
    MediaInfo_Char *mi_field_name;
    const MediaInfo_Char *mi_str;
    UNPACK_MI;

    mi_field_name = rstring_to_mediainfo_chars(field_name);

    mi_str = cMediaInfoAPI.get(mi->handle,
                            NUM2INT(stream_type),
                            NUM2INT(stream_id),
                            mi_field_name,
                            NUM2INT(request_type),
                            0);

    RSTRING_TO_MINFO_FREE(mi_field_name);

    rstr = mediainfo_chars_to_rstring(mi_str);
    return rstr;
}

static VALUE
mi_option(VALUE self, VALUE name, VALUE value)
{
    VALUE rstr;
    MediaInfo_Char *mi_name;
    MediaInfo_Char *mi_value;
    const MediaInfo_Char *mi_str;
    UNPACK_MI;

    mi_name  = rstring_to_mediainfo_chars(name);
    mi_value = rstring_to_mediainfo_chars(value);

    mi_str = cMediaInfoAPI.option(mi->handle, mi_name, mi_value);

    RSTRING_TO_MINFO_FREE(mi_value);
    RSTRING_TO_MINFO_FREE(mi_name);

    rstr = mediainfo_chars_to_rstring(mi_str);
    return rstr;
}

static VALUE
mi_state_get(VALUE self)
{
    UNPACK_MI;
    return INT2NUM(cMediaInfoAPI.state_get(mi->handle));
}

static VALUE
mi_count_get(VALUE self, VALUE stream_type, VALUE stream_id)
{
    UNPACK_MI;
    return INT2NUM(cMediaInfoAPI.count_get(mi->handle,
                                        NUM2INT(stream_type),
                                        NUM2INT(stream_id)));
}

static void
mi_check_binding(void *func, char *name)
{
    if (!func) {
        rb_raise(rb_eLoadError,
                 "Failed to bind MediaInfo_%s() from libmediainfo.so",
                 name);
    }
}

static const char *
stream_type_name(int type)
{
    switch(type) {
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


static VALUE
mi_stream_type(int argc, VALUE *argv, VALUE self)
{
    VALUE type_number;
    rb_scan_args(argc, argv, "10", &type_number);
    return rb_intern( stream_type_name(NUM2INT(type_number)) );
}


static VALUE
mi_num_streams(VALUE self, VALUE type)
{
    return mi_count_get(self, type, INT2NUM(-1));
}

static VALUE
mi_num_fields(VALUE self, VALUE type, VALUE stream_id)
{
    return mi_count_get(self, type, stream_id);
}

void
Init_mediainfo(void)
{
    MediaInfoDLL_Load();
    if (!MediaInfoDLL_IsLoaded()) {
        rb_raise(rb_eLoadError, "Mediainfo.so not loaded!\n");
    }

    /*
     * Classes
     */
#define MI_API(field, name)                                             \
    cMediaInfoAPI.field = JOIN(MediaInfo_,name);                        \
    mi_check_binding(cMediaInfoAPI.field, Q(name));

    MI_API(new,New);
    MI_API(delete,Delete);
    MI_API(open,Open);
    MI_API(close,Close);
    MI_API(inform,Inform);
    MI_API(geti,GetI);
    MI_API(get,Get);
    MI_API(option,Option);
    MI_API(state_get,State_Get);
    MI_API(count_get,Count_Get);
#undef MI_API

    cMediaInfo  = rb_define_class("MediaInfo", rb_cObject);
    rb_define_alloc_func(cMediaInfo, mi_allocate);

#define M(name, numargs)                                            \
    rb_define_method(cMediaInfo, Q(name), JOIN(mi_,name), numargs);
    M(initialize,  -1);
    M(open,        -1);
    M(close,        0);
    M(inform,       0);
    M(get_i,        4);
    M(get,          4);
    M(option,       2);
    M(state_get,    0);
    M(count_get,    2);
    M(stream_type, -1);
    M(num_streams,  1);
    M(num_fields,   2);
#undef M

    /*
     * Constants
     */
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
